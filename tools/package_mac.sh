#!/usr/bin/env bash
#
# Build a self-contained, portable bary.app (and a Bary.dmg) for macOS.
#
# The normal build links against Homebrew dylibs by absolute path
# (/opt/homebrew/...), so the .app only launches on a machine that happens to
# have those exact libraries installed at those exact paths. This script
# produces a bundle that carries its own copy of every non-system dylib (SDL2,
# SDL2_image, SDL2_mixer, lua, and all their transitive deps), so it runs on
# any arm64 Mac out of the box.
#
# Pipeline:
#   1. Configure + build the release tree (BARY_DEV=OFF -> assets copied into
#      the bundle instead of symlinked).
#   2. dylibbundler: copy every non-system dylib (including the transitive
#      chain) into Contents/Frameworks and rewrite the load commands from
#      /opt/homebrew/... to @executable_path/../Frameworks/...
#   2b. Bundle SDL3 by hand. Homebrew's "SDL2" is sdl2-compat, a shim that
#      dlopen()s SDL3 at runtime -- invisible to dylibbundler (see below).
#   3. Ad-hoc code-sign inside-out. Apple Silicon requires a valid signature;
#      step 2 invalidates the original, and an invalid signature is a common
#      cause of the "app is damaged" error.
#   4. Package into Bary.dmg with a drag-to-Applications layout.
#
# Prereq:  brew install dylibbundler
# Usage:   ./tools/package_mac.sh
#
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$REPO_ROOT/build-release"
APP="$BUILD_DIR/bary.app"
DMG="$BUILD_DIR/Bary.dmg"

if ! command -v dylibbundler >/dev/null 2>&1; then
  echo "error: dylibbundler not found. Install it with:  brew install dylibbundler" >&2
  exit 1
fi

echo "==> Configuring + building release tree (BARY_DEV=OFF)"
cmake -B "$BUILD_DIR" -S "$REPO_ROOT" -DBARY_DEV=OFF
# Delete the bundle so cmake is forced to relink the executable from scratch.
# dylibbundler rewrites the binary's load paths to @executable_path/... in place;
# if a re-run started from an already-rewritten binary, dylibbundler would wipe
# Frameworks (-od) and then find nothing to collect, producing an empty bundle.
# A fresh link guarantees the binary references /opt/homebrew again.
rm -rf "$APP"
cmake --build "$BUILD_DIR"

# Guard: the freshly built binary must reference Homebrew libs (i.e. it has not
# already been bundled). If not, the link state is wrong and bundling would
# silently produce a broken bundle.
if ! otool -L "$APP/Contents/MacOS/bary" | grep -q '/opt/homebrew'; then
  echo "error: freshly built binary has no /opt/homebrew references - unexpected" >&2
  echo "       link state; refusing to bundle. Try: rm -rf '$BUILD_DIR'" >&2
  exit 1
fi

echo "==> Bundling dylibs into Contents/Frameworks"
# -od overwrite dest dir, -b bundle deps, -cd create dir, -x fix this file,
# -d dest dir, -p rewrite load paths relative to the executable.
dylibbundler -od -b -cd \
  -x "$APP/Contents/MacOS/bary" \
  -d "$APP/Contents/Frameworks" \
  -p "@executable_path/../Frameworks/"

# The Homebrew "SDL2" is sdl2-compat: a shim that implements the SDL2 API on top
# of SDL3, which it loads at RUNTIME via dlopen("@loader_path/libSDL3.dylib") --
# not a linked dependency, so it never appears in the load-command table and
# dylibbundler can't see it. Copy SDL3 in by hand, next to libSDL2 (that's what
# @loader_path resolves to), with the unversioned symlink name the shim wants.
echo "==> Bundling SDL3 (dlopen'd by sdl2-compat; invisible to dylibbundler)"
SDL3_SRC="/opt/homebrew/opt/sdl3/lib/libSDL3.0.dylib"
if [ ! -f "$SDL3_SRC" ]; then
  echo "error: SDL3 not found at $SDL3_SRC (brew install sdl3)" >&2
  exit 1
fi
cp "$SDL3_SRC" "$APP/Contents/Frameworks/libSDL3.0.dylib"
chmod u+w "$APP/Contents/Frameworks/libSDL3.0.dylib"
install_name_tool -id "@executable_path/../Frameworks/libSDL3.0.dylib" \
  "$APP/Contents/Frameworks/libSDL3.0.dylib"
ln -sf libSDL3.0.dylib "$APP/Contents/Frameworks/libSDL3.dylib"

echo "==> Ad-hoc code-signing (inside-out: libs first, then the app)"
# -type f so we sign the real dylibs (incl. libSDL3.0.dylib) and skip symlinks.
find "$APP/Contents/Frameworks" -type f -name '*.dylib' -exec codesign --force --sign - {} +
codesign --force --sign - "$APP"

echo "==> Verifying bundle is self-contained"
# No load-command should point back at Homebrew, across the executable AND every
# bundled dylib (each dylib's own id was rewritten to @executable_path too).
if otool -L "$APP/Contents/MacOS/bary" "$APP"/Contents/Frameworks/*.dylib | grep -q '/opt/homebrew'; then
  echo "error: something still references /opt/homebrew - not portable:" >&2
  otool -L "$APP/Contents/MacOS/bary" "$APP"/Contents/Frameworks/*.dylib | grep '/opt/homebrew' >&2
  exit 1
fi
# SDL3 must be present or sdl2-compat aborts at startup with "Failed loading SDL3".
if [ ! -f "$APP/Contents/Frameworks/libSDL3.0.dylib" ]; then
  echo "error: libSDL3.0.dylib missing from bundle" >&2
  exit 1
fi
echo "    clean - self-contained, SDL3 bundled"

echo "==> Building $DMG"
STAGING="$(mktemp -d)"
cp -R "$APP" "$STAGING/Bary.app"
ln -s /Applications "$STAGING/Applications"
rm -f "$DMG"
hdiutil create -volname "Bary" -srcfolder "$STAGING" -ov -format UDZO "$DMG" >/dev/null
rm -rf "$STAGING"

echo
echo "Done."
echo "  App: $APP"
echo "  DMG: $DMG"
