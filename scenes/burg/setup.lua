-- BEHAVIORS

local function zinniaAutoMove(hostThing, args, eventName)
    originX, originY = table.unpack {args["originX"], args["originY"]}
    while true do
        _newTask(
            {
                {
                    type = "move",
                    destinationX = originX - 150,
                    destinationY = originY - 150
                },
            }, eventName, hostThing
        )
        coroutine.yield()
        _newTask(
            {
                {
                    type = "move",
                    destinationX = originX + 35,
                    destinationY = originY - 150
                },
            }, eventName, hostThing
        )
        coroutine.yield()
        _newTask(
            {
                {
                    type = "move",
                    destinationX = originX,
                    destinationY = originY
                },
            }, eventName, hostThing
        )
        coroutine.yield()
    end
end

-- EVENTS

local function zinniaTalkA(hostThing, args, eventName)
    _newTask(
        {
            {
                type = "phrase",
                text = "Didn't I tell you not to come around here",
                x = 150,
                y = 150,
                width = 400,
                height = 100,
                scrollType = "continuous",
                gridLimitsX = 1000,
                gridLimitsY = 1000,
                frames = 250,
            }
        }, eventName, hostThing
    )
end

local function zinniaTalkB(hostThing, args, eventName)
    _newTask({{
        type = "pauseMoves",
        all = true,
    }}, eventName, hostThing)
    _newTask(
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
        }, eventName, hostThing
    )
    coroutine.yield()
    _newTask(
        {
            -- {
            --     type = "move",
            --     thingName = "otherZinnia",
            --     offsetX = -300,
            --     offsetY = 300
            -- },
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
        }, eventName, hostThing
    )
    coroutine.yield()
    _newTask({{
        type = "pauseMoves",
        unpause = true,
        all = true,
    }}, eventName, hostThing)
end

--     IDEAS FOR FUTURE OF EDITOR AND DATA STORAGE:
--     in thing editor you can build a thing, then export it as a lua table

local thingDefs = {
    playerZinnia = {
        name = "playerZinnia",
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
                sourceY = 0
            }
        },
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
                sourceY = 0
            }
        },
        obstructionData = {},
        components = {
            {
                type = "moveAnimate",
            },
            {
                type = "standardCollider",
                eventNames = {
                    "talk_1",
                    "talk_2"
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
            -- autoMove = {
            --     type = "custom",
            --     customCoroutine = zinniaAutoMove
            -- },
            talk_1 =  { -- This has the same effect as zinniaTalk, but is stored as data
                type = "simpleMessages",
                phrases = {
                    {
                        text = "Hey what's happening bro",
                        x = 300,
                        y = 150,
                        width = 400,
                        height = 100,
                        scrollType = "continuous",
                        gridLimitsX = 1000,
                        gridLimitsY = 1000,
                    },
                    {
                        text = "Didn't I tell you not to come around here",
                        x = 150,
                        y = 150,
                        width = 400,
                        height = 100,
                        scrollType = "continuous",
                        gridLimitsX = 1000,
                        gridLimitsY = 1000,
                    },
                }
            },
            talk_2 = {
                type = "custom",
                customCoroutine = zinniaTalkB
            },
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
                sourceY = 0
            }
        },
        obstructionData = {},
        components = {
            {
                type = "moveAnimate",
            },
            {
                type = "standardCollider",
                eventNames = {
                    "fz_1",
                    "fz_2"
                }
            },
            {
                type = "follow",
                targetName = "testPlayer",
                tolerance = 40
            }
        },
        events = {
            fz_1 =  {
                type = "custom",
                customCoroutine = zinniaTalkA
            },
            fz_2 = {
                type = "custom",
                customCoroutine = zinniaTalkB
            }
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
                        layer = 0
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
                        layer = 0,
                        active = false
                    }
                },
                obstructionData = {},
                components = {
                    {
                        type = "standardCollider",
                        eventNames = {
                            "open",
                        }
                    },
                },
                events = {
                    open = {
                        type = "openDoor",
                        -- locked = true
                    }
                }
            },
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

return { thingDefs, eventDefinitions }