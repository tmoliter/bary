-- BEHAVIORS

local function zinniaAutoMove(hostScene, hostThing, args)
    originX, originY = table.unpack {args["originX"], args["originY"]}
    while true do
        _updateMoveTarget(originX - 150, originY - 50, hostThing)
        coroutine.yield()
        _updateMoveTarget(originX + 35, originY + 150, hostThing)
        coroutine.yield()
        _updateMoveTarget(originX, originY, hostThing)
        coroutine.yield()
    end
end

behaviorDefinitions = {
    otherZinnia = {
        autoMove = zinniaAutoMove
    }
}

-- EVENTS

local function zinniaTalkA(hostScene, hostThing, args, eventName)
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
                frames = 259
            }
        }, eventName, hostThing, hostScene
    )
end

local function zinniaTalkB(hostScene, hostThing, args, eventName)
    _newTask(
        {
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
                frames = 100
            }
        }, eventName, hostThing, hostScene
    )
    coroutine.yield()
    local thingData = _getThingData(hostThing)
    _newTask(
        {
            {
                type = "move",
                thingPointer = hostThing,
                destinationX = thingData["x"] - 150,
                destinationY = thingData["y"] + 100,
            },
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
                frames = 75
            }
        }, eventName, hostThing, hostScene
    )
end

eventDefinitions = {
    followZinnia = { -- name of thing
        interact =  {
            { -- name of collidable
                type = "custom",
                customCoroutine = zinniaTalkA
            },
            { -- name of collidable
                type = "custom",
                customCoroutine = zinniaTalkB
            }
        }
    },
    -- NOTE: Non-custom event definitions maybe should be able to be stored on the serialized Thing in map.lua
    otherZinnia = {
        interact =  {
            -- { -- name of collidable
            --     type = "custom",
            --     customCoroutine = zinniaTalkB
            -- },
            { -- This has the same effect as zinniaTalk, but is stored as data
                type = "simpleMessages",
                args = {
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
                }
            }
        }
    }
}

return eventDefinitions
