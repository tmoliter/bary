-- Bundles: declarative thingDef archetypes that expand into the primitives the
-- engine consumes, so an author writes one high-level block instead of wiring up
-- components/events/sprite state by hand.
--
-- Currently just `openable` (doors, chests). When a second bundle lands (e.g. a
-- standard NPC), the shared traversal/guard/recursion can be factored into a
-- registry — not worth the abstraction for one handler yet.
--
-- An openable bundle touches three phases:
--   expandOpenableDefs  def-time,   before populate: synthesize collider + open event
--   applyOpenable       spawn-time,  per instance:    set sprite state, strip colliders
--   openableOpenTasks   runtime,     on open:         the "become open" subtasks

local bundles = {}

-- def-time -------------------------------------------------------------------
-- Expand an `openable` declaration into a standardCollider (so it can be
-- triggered) and an `open` event (with the opening effects). Guarded because
-- thingDefs are cached and reused across loads. Recurses into subThings so nested
-- openables (e.g. a door on a building) are handled too.
local function expandOpenable(thingDef)
    if thingDef.subThings ~= nil then
        for _,subThing in ipairs(thingDef.subThings) do
            expandOpenable(subThing)
        end
    end

    local openable = thingDef.openable
    if openable == nil or thingDef._openableExpanded then
        return
    end

    thingDef._openableExpanded = true

    -- the open event: opening effects + a back-reference so the event's runtime
    -- sprite/collider changes come from the same block (openableOpenTasks)
    local openEvent = {
        type = "open",
        openable = openable,
        triggerDelay = openable.triggerDelay,
        closeAfter = openable.closeAfter,
        locked = openable.locked,
    }
    if openable.onOpen ~= nil then
        for k,v in pairs(openable.onOpen) do openEvent[k] = v end
    end
    thingDef.events = thingDef.events or {}
    thingDef.events.open = openEvent

    -- the collider that fires it
    local catalyst = openable.catalyst or "interactable"
    thingDef.components = thingDef.components or {}
    thingDef.components[#thingDef.components + 1] = {
        type = "standardCollider",
        interactable = catalyst == "interactable",
        trigger = catalyst == "trigger",
        eventNames = { "open" },
    }
end

-- Runs before populate so the synthesized components/events are in place.
function bundles.expandOpenableDefs(thingDefs)
    for _,thingDef in pairs(thingDefs) do
        expandOpenable(thingDef)
    end
end

-- spawn-time -----------------------------------------------------------------
-- Set an openable instance's sprite state for its current open/closed state, and
-- strip colliders when open so it can't be re-opened. Copies the tables it mutates
-- so the shared cached thingDef is never touched. State comes from the persisted
-- delta (spawn.openState) or the def's `initial`.
function bundles.applyOpenable(spawn)
    local openable = spawn.openable
    if openable == nil then
        return
    end
    local state = spawn.openState or openable.initial or "closed"
    spawn.openState = state

    if openable.sprites ~= nil and spawn.spriteDataVector ~= nil then
        local activeSet = {}
        for _,zeroIndex in ipairs(openable.sprites[state] or {}) do
            activeSet[zeroIndex] = true
        end
        local sprites = deepcopy(spawn.spriteDataVector)
        for i,sprite in ipairs(sprites) do
            sprite.active = activeSet[i - 1] == true
        end
        spawn.spriteDataVector = sprites
    end

    if state == "open" and openable.collidersWhenOpen ~= true and spawn.components ~= nil then
        local components = {}
        for _,component in ipairs(spawn.components) do
            if component.type ~= "standardCollider" then
                components[#components + 1] = component
            end
        end
        spawn.components = components
    end
end

-- runtime --------------------------------------------------------------------
-- The "become open" subtasks: show the open sprites, and (unless colliders are kept
-- when open) disable colliders so the thing can't be re-opened. Sourced from the
-- same openable block that drives spawn, instead of hardcoded indices.
function bundles.openableOpenTasks(openable)
    local tasks = {{
        type = "setActiveSprites",
        sprites = (openable.sprites and openable.sprites.open) or { 0 }
    }}
    if openable.collidersWhenOpen ~= true then
        tasks[#tasks + 1] = {
            type = "disableColliders",
            obstructions = true
        }
    end
    return tasks
end

return bundles
