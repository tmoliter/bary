local resources = {
    textures = {
        sailorshack = "sheets/Burg/SailorShack",
        genrl = "x",
        pinkbox = "menus/blankPink",
        pinkboxheader = "menus/pinkInventoryHeader",
        pinkinventoryfooter = "menus/pinkInventoryFooter"
    },
}

deepmerge(resources, require('barycenter.resources.editor'))
deepmerge(resources, require('barycenter.resources.characters'))

return resources