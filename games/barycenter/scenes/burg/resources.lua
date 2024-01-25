local resources = {
    textures = {
        sailorshack = "sheets/Burg/SailorShack",
        genrl = "x",
        pinkbox = "menus/blankPink",
        pinkboxheader = "menus/pinkInventoryHeader",
        pinkinventoryfooter = "menus/pinkInventoryFooter"
    },
}

deepmerge(resources, require(GAME_PATH .. '.resources.editor'))
deepmerge(resources, require(GAME_PATH .. '.resources.characters'))

return resources