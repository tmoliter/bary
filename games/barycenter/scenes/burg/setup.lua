local Resources = require('scripts.resourceobject')
local resources = Resources.new({
    background = "burg",
    ownTextures = {
        burg = "backgrounds/Burg",
        genrl = "sheets/Burg/genrl",
        sailorshack = "sheets/Burg/SailorShack",
        zinnia = "sheets/SDL_TestSS",
        chest = "sheets/Burg/chest"
    },
})

local zinniaTalkB = {
    type = "sequentialTasks",
    pauseAllMoves = true,
    tasks = {
        {
            {
                type = "move",
                offsetX = -100,
                offsetY = 50
            },
            {
                type = "phrase",
                text = "doodoo",
                x = 30,
                y = 40, 
                width = 80,
                height = 50,
                scrollType = "continuous",
                gridLimitsX = 100,
                gridLimitsY = 100,
                frames = 125,
            }
        },
        {
            {
                type = "phrase",
                text = "poopoo",
                x = 300,
                y = 100, 
                width = 100,
                height = 50,
                scrollType = "continuous",
                gridLimitsX = 1000,
                gridLimitsY = 1000,
                frames = 125,
            }
        }
    }
}

local function unlockQuest(hostThing, args)
    args["gameState"]:updateQuest("some.damn.quest", "completed")
end


local function doorCondition(hostThing, args)
    for _,inv in pairs(args["gameState"]["inventories"]) do
        if inv:count("oolong") > 36 then
            return true
        end
    end
    return false
end


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
    otherZinnia = {
        name = "otherZinnia",
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
        components = {
            {
                type = "moveAnimate",
            },
            {
                type = "standardCollider",
                interactable = true,
                eventNames = {
                    "talk_1",
                    "talk_2",
                    "unlock",
                }
            },
            {
                type = "autoMove",
                eventName = "autoMove",
            }
        },
        events = {
            autoMove = {
                type = "randomAutoMove",
                variance = 100
            },
            talk_1 =  {
                type = "sequentialTasks",
                pauseAllMoves = true,
                tasks = {
                    {{
                        type = "phrase",
                        text = "Hey what's happening bro",
                        x = 300,
                        y = 150,
                        width = 400,
                        height = 100,
                    }},
                    {{
                        type = "phrase",
                        text = "Didn't I tell you not to come around here",
                        x = 150,
                        y = 150,
                        width = 400,
                        height = 100,
                    }},
                }
            },
            talk_2 = zinniaTalkB,
            unlock = { type = "custom", customCoroutine = unlockQuest }
        }
    },
    followZinnia = {
        name = "followZinnia",
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
        components = {
            {
                type = "moveAnimate",
            },
            {
                type = "standardCollider",
                interactable = true,
                eventNames = { "fz_1" }
            },
            {
                type = "follow",
                targetName = "testPlayer",
                tolerance = 40
            }
        },
        events = {
            fz_1 = zinniaTalkB
        }
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
                    catalyst = "trigger",
                    sprites = { open = { 0 }, closed = { 1 } },
                    obstructionsWhenOpen = false,
                    triggerDelay = 30,
                    closeAfter = true,
                    onOpen = {
                        portal = {
                            relativeX = -25,
                            relativeY = -131,
                            newLayer = 2,
                            newScene = "testSceneTwo"
                        },
                    },
                    locked = {
                        message = "Locked, fuckface.",
                        active = true,
                        condition = {
                            quest = {
                                ["some.damn.quest"] = "completed"
                            },
                            item = {
                                name = "oolong",
                                quantity = 38
                            },
                            -- func = doorCondition
                            permanent = true,
                        }
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
    chest = {
        name = "chest",
        spriteDataVector = {
            {
                width = 41,
                height = 36,
                sourceX = 40,
                sourceY = 0,
                xOffset = 0,
                yOffset = -36,
                textureName = "chest",
                renderOffset = 0,
                layer = 0
            },
            {
                width = 40,
                height = 36,
                sourceX = 0,
                sourceY = 0,
                xOffset = 0,
                yOffset = -36,
                textureName = "chest",
                renderOffset = 0,
                layer = 0
            }
        },
        obstructionData = {
            {
                rays = {
                        {
                            aY = -7,
                            bY = -7,
                            aX = 7,
                            bX = 39
                        },
                        {
                            aY = -7,
                            bY = -1,
                            aX = 7,
                            bX = 1
                        },
                        {
                            aY = -7,
                            bY = -1,
                            aX = 39,
                            bX = 33
                        },
                        {
                            aY = -1,
                            bY = -1,
                            aX = 0,
                            bX = 32
                        }
                    },
                    layer = 0,
            }
        },
		interactableData = {
			{
				rays = {
					{
						aY = -7,
						bY = -7,
						aX = 7,
						bX = 39
					},
					{
						aY = -7,
						bY = -1,
						aX = 7,
						bX = 1
					},
					{
						aY = -7,
						bY = -1,
						aX = 39,
						bX = 33
					},
					{
						aY = -1,
						bY = -1,
						aX = 0,
						bX = 32
					}
				},
				name = "opensesame",
				layer = 0,
                eventNames = { "open" },
			}
		},
        openable = {
            initial = "closed",
            persist = true,
            sprites = { open = { 0 }, closed = { 1 } },
            triggerDelay = 30,
            closeAfter = false,
            onOpen = {
                receiveItem = { name = "oolong", amount = 6 },
            },
        },
    }
}

return { resources, thingDefs, sceneEvents }