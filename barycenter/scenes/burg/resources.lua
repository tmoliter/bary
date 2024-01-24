local resources = {
    textures = {
        sailorshack = "assets/sheets/Burg/SailorShack.png",
        genrl = "assets/x.png",
        pinkbox = "assets/menus/blankPink.png",
        pinkboxheader = "assets/menus/pinkInventoryHeader.png",
        pinkinventoryfooter = "assets/menus/pinkInventoryFooter.png"
    },
}

deepmerge(resources, require('barycenter.resources.editor'))
deepmerge(resources, require('barycenter.resources.characters'))

return resources