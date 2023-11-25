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

behaviors = {
    otherZinnia = {
        autoMove = coroutine.create(zinniaAutoMove)
    }
}

-- EVENTS

-- local function zinniaTalk(hostThing)
--     _updateMoveSpeed(0, hostThing)
--     _phrase(
--         {
--             text = "Hey what's happening bro",
--             x = 150,
--             y = 150,
--             width = 400,
--             height = 100,
--             scrollType = "continuous",
--             gridLimitsX = 1000,
--             gridLimitsY = 1000,
--         }
--     )
--     coroutine.yield()
--     _phrase(
--         {
--             text = "Didn't I tell you not to come around here",
--             x = 150,
--             y = 150,
--             width = 400,
--             height = 100,
--             scrollType = "continuous",
--             gridLimitsX = 1000,
--             gridLimitsY = 1000,
--         }
--     )
--     coroutine.yield()
--     _updateMoveSpeed(1, hostThing)
-- end


-- eventDefinitions = {
--     otherZinnia = { -- name of thing
--         interact =  { -- name of collidable
--             type = "custom",
--             customCoroutine = zinniaTalk
--         }
--     },
--     -- NOTE: Non-custom event definitions should be able to be stored on the serialized Thing in map.lua
--     somebodyElse = {
--         interact =  { -- This has the same effect as zinniaTalk, but is stored as data
--             type = "simpleMessages",
--             phrases = {
--                 {
--                     text = "Hey what's happening bro",
--                     x = 150,
--                     y = 150,
--                     width = 400,
--                     height = 100,
--                     scrollType = "continuous",
--                     gridLimitsX = 1000,
--                     gridLimitsY = 1000,
--                 },
--                 {
--                     text = "Didn't I tell you not to come around here",
--                     x = 150,
--                     y = 150,
--                     width = 400,
--                     height = 100,
--                     scrollType = "continuous",
--                     gridLimitsX = 1000,
--                     gridLimitsY = 1000,
--                 },
--             }
--         }
--     }
-- }

-- activeEvents = {}

-- -- NOTE: These sorts of functions should be moved somewhere scene agnostic and re-usable

function doAutoMove(originX, originY, name, hostThing)
    if coroutine.status(behaviors[name]["autoMove"]) ~= 'dead' then
        coroutine.resume(behaviors[name]["autoMove"], originX, originY, hostThing) 
    end
end

-- --

-- local function simpleMessages(eventDef)
--     _updateMoveSpeed(0, hostThing)
--     for k,v in ipairs(eventDef["Phrases"])
--         _phrase(v)
--         coroutine.yield()
--     end
--     _updateMoveSpeed(0, hostThing)
-- end

-- --

-- local function doEvent(collidableName, thingName, hostThing)
--     local eventDefinition = eventDefinitions[thingName][collidableName]
--     if activeEvents[thingName][collidableName] == nil
--         activeEvents[thingName][collidableName] = { timesInvoked = 0 }
--     end
--     if event["type"] == "custom" then
--         activeEvents[thingName][collidableName]["event"] = coroutine.create(eventDefinition["customCoroutine"])
--     else if event["type"] == "simpleMessages" then
--         activeEvents[thingName][collidableName]["event"] = coroutine.create(simpleMessages(eventDef))
--     end
-- end
        
-- local function resumeEvent(collidableName, thingName, hostThing)
--     -- Event has never been invoked
--     if activeEvents[thingName][collidableName] == nil then
--         doEvent(collidableName, thingName, hostThing)
--         return 0
--     end
--     -- Event has just now finished
--     if coroutine.status(activeEvents[thingName][collidableName]) == 'dead' then
--         activeEvents[thingName][collidableName]["event"] = nil
--         activeEvents[thingName][collidableName]["timesInvoked"] = events[thingName][collidableName]["timesInvoked"] + 1
--         return 1
--     end
--     -- Event is in progress
--     coroutine.resume(events[thingName][collidableName], hostThing)
--     return 0
-- end