local Resources = require('scripts.resourceobject')
local resources = Resources.new({
    background = "tg",
    ownTextures = {
        tg = "backgrounds/tg",
        genrl = "sheets/Burg/genrl",
        sailorshack = "sheets/Burg/SailorShack",
        zinnia = "sheets/Zinnia"
    },
})

local globalEvents = require(GAME_PATH .. ".definitions.globalEvents")
local sceneEvents = {
    inventoryMenu = {
        type = "custom",
        customCoroutine = globalEvents.inventoryMenu
    }
}

local thingDefs = {
    zinnia = {
        name = "zinnia",
        spriteDataVector = {
            {
                xOffset = 0,
                height = 0,
                layer = 0,
                textureName = "zinnia",
                renderOffset = 0,
                width = 0,
                yOffset = 0,
                sourceX = 0,
                sourceY = 0,
                sheetColumns = 9,
                sheetRows = 4
            }
        },
        obstructionData = {},
        fieldPlayer = true
    },
    sailorShack = {
        name = "Sailor Shack",
        spriteDataVector = {
            {
                xOffset = -145,
                height = 117,
                layer = 0,
                textureName = "sailorshack",
                renderOffset = 0,
                width = 291,
                yOffset = -117,
                sourceX = 0,
                sourceY = 0
            },
            {
                xOffset = 146,
                height = 183,
                layer = 0,
                textureName = "sailorshack",
                renderOffset = 33,
                width = 33,
                yOffset = -181,
                sourceX = 324,
                sourceY = 57
            },
            {
                xOffset = -145,
                height = 97,
                layer = 2,
                textureName = "sailorshack",
                renderOffset = 0,
                width = 324,
                yOffset = -214,
                sourceX = 0,
                sourceY = 143
            },
            {
                xOffset = -138,
                height = 26,
                layer = 1,
                textureName = "sailorshack",
                renderOffset = 0,
                width = 309,
                yOffset = -214,
                sourceX = 0,
                sourceY = 117
            },
            {
                xOffset = -69,
                height = 57,
                layer = 2,
                textureName = "sailorshack",
                renderOffset = 0,
                width = 55,
                yOffset = -250,
                sourceX = 291,
                sourceY = 0
            },
            {
                xOffset = 85,
                height = 21,
                layer = 2,
                textureName = "sailorshack",
                renderOffset = 0,
                width = 29,
                yOffset = -215,
                sourceX = 291,
                sourceY = 58
            }
        },
        obstructionData = {
            {
                rays = {
                    {aX = -106, aY = -206, bY = -168, bX = -144},
                    {aX = 165, aY = -206, bY = -169, bX = 128},
                    {aX = 127, aY = -168, bY = -168, bX = -143},
                    {aX = -106, aY = -206, bY = -206, bX = 165}
                },
                layer = 2
            },
            {
                rays = {
                    {aX = 144, aY = -1, bY = -1, bX = 29},
                    {aX = 146, aY = -1, bY = -33, bX = 178},
                    {aX = -144, aY = -1, bY = -32, bX = -113},
                    {aX = -113, aY = -32, bY = -32, bX = 176},
                    {aX = -29, aY = -1, bY = -1, bX = -144}
                },
                layer = 0
            }
        },
        subThings = {
            {
                name = "SailorShackDoor",
                y = -61,
                x = -27,
                spriteDataVector = {
                    {
                        width = 56,
                        height = 60,
                        sourceY = 60,
                        xOffset = 0,
                        yOffset = 0,
                        textureName = "sailorshack",
                        renderOffset = -1,
                        sourceX = 357,
                        layer = 0,
                        active = false
                    },
                    {
                        width = 56,
                        height = 60,
                        sourceY = 0,
                        xOffset = 0,
                        yOffset = 0,
                        textureName = "sailorshack",
                        renderOffset = -1,
                        sourceX = 357,
                        layer = 0
                    }
                },
                obstructionData = {},
                openable = {
                    initial = "closed",
                    persist = false,
                    catalyst = { "trigger", "interactable" },
                    sprites = { open = { 0 }, closed = { 1 } },
                    obstructionsWhenOpen = false,
                    triggerDelay = 30,
                    closeAfter = true,
                    onOpen = {
                        portal = {
                            relativeX = 30,
                            relativeY = 90,
                            newLayer = 0,
                            newScene = "burg"
                        },
                    },
                }
            }
        }
    },
    genrlStore = {
        name = "genrlStore",
        spriteDataVector = {
            {
                xOffset = -119,
                height = 145,
                layer = 0,
                textureName = "genrl",
                renderOffset = 0,
                width = 236,
                yOffset = -145,
                sourceX = 0,
                sourceY = 0
            }
        },
        obstructionData = {
            {rays = {{aX = -11, aY = -2, bY = -2, bX = -119}, {aX = 119, aY = -2, bY = -2, bX = 11}}, layer = 0}
        }
    },
}

return { resources, thingDefs, sceneEvents }