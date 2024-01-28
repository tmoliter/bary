local Resources = require('state.resources')
local characters = require(GAME_PATH .. '.resources.characters')
local resources = Resources.new({
    ownTextures = getMerge({
        {
            sailorshack = "sheets/Burg/SailorShack",
            genrl = "x",
            pinkbox = "menus/blankPink",
            pinkboxheader = "menus/pinkInventoryHeader",
            pinkinventoryfooter = "menus/pinkInventoryFooter",
            zinnia = characters.zinnia
        },
        -- require(GAME_PATH .. '.resources.characters').textures,
    }),
    baseTextures = require('base.resources').editor.textures,
})

return resources