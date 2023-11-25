-- BEHAVIORS

local function zinniaAutoMove(originX, originY, hostThing)
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
    _updateMoveSpeed(0, hostThing)
    _phrase(
        {
            text = "Hey what's happening bro",
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
    _updateMoveSpeed(1, hostThing)
end


eventDefinitions = {
    otherZinnia = { -- name of thing
        interact =  { -- name of collidable
            type = "custom",
            customCoroutine = zinniaTalk
        }
    },
    -- NOTE: Non-custom event definitions maybe should be able to be stored on the serialized Thing in map.lua
    somebodyElse = {
        interact =  { -- This has the same effect as zinniaTalk, but is stored as data
            type = "simpleMessages",
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

activeEvents = {}
activeBehaviors = {}

-- NOTE: These sorts of functions should be moved somewhere scene agnostic and re-usable

function beginAutoMove(originX, originY, hostThingName, hostThing)
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

function doAutoMove(hostThing)
    local autoMove = activeBehaviors[hostThing]["autoMove"]
    if coroutine.status(autoMove["def"]) ~= 'dead' then
        coroutine.resume(
            autoMove["def"],  
            autoMove["originX"],
            autoMove["originY"],
            hostThing
        )
        autoMove["timesInvoked"] = autoMove["timesInvoked"] + 1
    end
    print(activeBehaviors[hostThing]["autoMove"]["timesInvoked"])
    print(hostScene)
end

--

local function simpleMessages(eventDef)
    _updateMoveSpeed(0, hostThing)
    for k,v in ipairs(eventDef["Phrases"]) do
        _phrase(v)
        coroutine.yield()
    end
    _updateMoveSpeed(0, hostThing)
end

--

local function doEvent(collidableName, thingName, hostThing)
    if eventDefinitions[thingName] == nil or eventDefinitions[thingName][collidableName] == nil then
        return 0
    end
    local eventDefinition = eventDefinitions[thingName][collidableName]
    if activeEvents[thingName][collidableName] == nil then
        activeEvents[thingName][collidableName] = { timesInvoked = 0 }
    end
    if event["type"] == "custom" then
        activeEvents[thingName][collidableName]["event"] = coroutine.create(eventDefinition["customCoroutine"])
    elseif event["type"] == "simpleMessages" then
        activeEvents[thingName][collidableName]["event"] = coroutine.create(simpleMessages(eventDef))
    end
    return 1
end
        
local function resumeEvent(collidableName, thingName, hostThing)
    -- Event has never been invoked
    if activeEvents[thingName][collidableName] == nil then
        doEvent(collidableName, thingName, hostThing)
        return 0
    end
    -- Event has just now finished
    if coroutine.status(activeEvents[thingName][collidableName]) == 'dead' then
        activeEvents[thingName][collidableName]["event"] = nil
        activeEvents[thingName][collidableName]["timesInvoked"] = events[thingName][collidableName]["timesInvoked"] + 1
        return 1
    end
    -- Event is in progress
    coroutine.resume(events[thingName][collidableName], hostThing)
    return 0
end
