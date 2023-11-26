-- BEHAVIORS

local function zinniaAutoMove(hostThing, originX, originY)
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

local function zinniaTalk(hostThing)
    -- _updateMoveSpeed(0, hostThing)
    _phrase(
        {
            text = "poopoo",
            x = 150,
            y = 150,
            width = 200,
            height = 100,
            scrollType = "continuous",
            gridLimitsX = 100,
            gridLimitsY = 100,
        }
    )
    coroutine.yield()
    _phrase(
        {
            text = "Didn't I tell you not to come around here",
            x = 150,
            y = 150,
            width = 400,
            height = 100,
            scrollType = "continuous",
            gridLimitsX = 1000,
            gridLimitsY = 1000,
        }
    )
    coroutine.yield()
    -- _updateMoveSpeed(1, hostThing)
end


eventDefinitions = {
    followZinnia = { -- name of thing
        interact =  { -- name of collidable
            type = "custom",
            customCoroutine = zinniaTalk
        }
    },
    -- NOTE: Non-custom event definitions maybe should be able to be stored on the serialized Thing in map.lua
    otherZinnia = {
        interact =  { -- This has the same effect as zinniaTalk, but is stored as data
            type = "simpleMessages",
            args = {
                phrases = {
                    {
                        text = "Hey what's happening bro",
                        x = 150,
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

activeEvents = {}
activeBehaviors = {}

-- NOTE: These sorts of functions should be moved somewhere scene agnostic and re-usable

function beginAutoMove(hostScene, hostThing, originX, originY, hostThingName)
    local autoMove = {
        def = coroutine.create(behaviorDefinitions[hostThingName]["autoMove"]),
        originX = originX,
        originY = originY,
        timesInvoked = 0
    }
    if activeBehaviors[hostThing] == nil then
        activeBehaviors[hostThing] = {
            autoMove = autoMove
        }
    else
        activeBehaviors[hostThing]["autoMove"] = autoMove
    end
end

function doAutoMove(hostScene, hostThing)
    local autoMove = activeBehaviors[hostThing]["autoMove"]
    if coroutine.status(autoMove["def"]) ~= 'dead' then
        coroutine.resume(
            autoMove["def"],
            hostThing,
            autoMove["originX"],
            autoMove["originY"]
        )
        autoMove["timesInvoked"] = autoMove["timesInvoked"] + 1
    end
    print(activeBehaviors[hostThing]["autoMove"]["timesInvoked"])
    print(hostThing)
end

--

local function simpleMessages(hostScene, hostThing, args)
    -- _updateMoveSpeed(0, hostThing)
    print(args)
    for k,v in ipairs(args["phrases"]) do
        _phrase(v)
        coroutine.yield()
    end
    -- _updateMoveSpeed(0, hostThing)
end

--
        
local function resumeEvent(hostScene, hostThing, collidableName, args)
    -- Event is not active
    if activeEvents[hostThing] == nil or 
        activeEvents[hostThing][collidableName] == nil or
        activeEvents[hostThing][collidableName]["coroutine"] == nil then
        return 0
    end

    activeEvent = activeEvents[hostThing][collidableName]

    -- Event is already finished
    if coroutine.status(activeEvent["coroutine"]) == 'dead' then
        activeEvent["coroutine"] = nil
        return 0
    end

    -- Event is in progress
    coroutine.resume(activeEvent["coroutine"], hostScene, hostThing, args or activeEvent["args"])
    if coroutine.status(activeEvent["coroutine"]) == 'dead' then
        activeEvent["coroutine"] = nil
        return
    end
    return 1
end


function doEvent(hostScene, hostThing, thingName, collidableName, args)
    print(thingName)
    if eventDefinitions[thingName] == nil or eventDefinitions[thingName][collidableName] == nil then
        return 0
    end

    if activeEvents[hostThing] == nil then
        activeEvents[hostThing] = {}
    end

    local eventDefinition = eventDefinitions[thingName][collidableName]

    if activeEvents[hostThing][collidableName] == nil then
        activeEvents[hostThing][collidableName] = { timesInvoked = 0 }
    end

    activeEvent = activeEvents[hostThing][collidableName]
    activeEvent["args"] = args or eventDefinition["args"]

    if eventDefinition["type"] == "custom" then
        activeEvent["coroutine"] = coroutine.create(eventDefinition["customCoroutine"])
    elseif eventDefinition["type"] == "simpleMessages" then
        activeEvent["coroutine"] = coroutine.create(simpleMessages)
    end
    activeEvent["timesInvoked"] = activeEvent["timesInvoked"] + 1

    return resumeEvent(hostScene, hostThing, collidableName, args or activeEvent["args"])
end