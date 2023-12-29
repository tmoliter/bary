local standardBehaviors = require('standardDefinitions.behaviors')

local activeEvents = {}
local activeBehaviors = {}
local eventDefinitions = {}
local behaviorDefinitions = {}

local function resumeEvent(hostThing, eventName)
    -- Event is not active
    if activeEvents[eventName] == nil or
        activeEvents[eventName]["coroutine"] == nil then
        return true
    end

    activeEvent = activeEvents[eventName]

    -- Event is already finished
    if coroutine.status(activeEvent["coroutine"]) == 'dead' then
        activeEvent["coroutine"] = nil
        return true
    end

    -- Event is in progress
    coroutine.resume(activeEvent["coroutine"], hostThing, activeEvent["args"], eventName)
    if coroutine.status(activeEvent["coroutine"]) == 'dead' then
        activeEvent["coroutine"] = nil
        return true
    end
    return false
end


local function beginEvent(hostThing, thingName, collidableName, args)
    if eventDefinitions[thingName] == nil or eventDefinitions[thingName][collidableName] == nil then
        return 0
    end

    for i, eventDefinition in ipairs(eventDefinitions[thingName][collidableName]) do
        local eventName = thingName .. "_" .. collidableName
        
        -- Event has never been invoked
        if activeEvents[eventName] == nil then
            activeEvents[eventName] = { timesInvoked = 0 }
        end

        local activeEvent = activeEvents[eventName]

        -- Event is already in progress
        if activeEvent["coroutine"] ~= nil then
            return
        end

        
        activeEvent["args"] = args or eventDefinition["args"]

        if eventDefinition["type"] == "custom" then
            activeEvent["coroutine"] = coroutine.create(eventDefinition["customCoroutine"], hostThing, activeEvent["args"])
        elseif eventDefinition["type"] == "simpleMessages" then
            activeEvent["coroutine"] = coroutine.create(simpleMessages, hostThing, activeEvent["args"])
        end
        activeEvent["timesInvoked"] = activeEvent["timesInvoked"] + 1

        -- return task data for C++ here?
        -- Data will be an event name, a list of subtasks, and a table of data for each subtask
        resumeEvent(hostThing, eventName)
    end
end

local function simpleMessages(hostThing, args, eventName)
    local index = 1
    for k,v in ipairs(args["phrases"]) do
        v["type"] = "phrase"
        _newTask({ v }, eventName, hostThing)
        if index < #args["phrases"] then
            index = index + 1
            coroutine.yield()
        end
    end
end


local function beginBehavior(hostThing, args)
    local behaviorType, thingName, standardBehavior = table.unpack {args["behaviorType"], args["thingName"], args["standardBehavior"]}
    
    local behaviorDef
    for k,v in pairs(standardBehaviors) do
        if standardBehavior == k then behaviorDef = v end
    end
    if behaviorDef == nil then
        behaviorDef = behaviorDefinitions[thingName][behaviorType]
    end
    if behaviorDef == nil then
        print("no behavior def could be found")
        return
    end

    local behavior = {
        def = coroutine.create(behaviorDef),
        args = args
    }
    if activeBehaviors[hostThing] == nil then
        local x = {
            [behaviorType] = behavior
        }
        activeBehaviors[hostThing] = {
            [behaviorType] = behavior
        }
    else
        activeBehaviors[hostThing][behaviorType] = behavior
    end
    doBehavior(hostThing, behaviorType)
end

local function doBehavior(hostThing, behaviorType)
    local behavior = activeBehaviors[hostThing][behaviorType]
    if coroutine.status(behavior["def"]) ~= 'dead' then
        coroutine.resume(
            behavior["def"],
            hostThing,
            behavior["args"]
        )
    end
end

local function populateDefinitions(definitions)
    eventDefinitions, behaviorDefinitions = table.unpack(definitions)
end

return {
    resumeEvent,
    beginEvent,
    simpleMessages,
    beginBehavior,
    doBehavior,
    populateDefinitions
}

--[[

DO WE NEED TO DECOUPLE EVENTS FROM COLLIDABLES EARLY, SO THAT THINGS LIKE BUTTON PRESSES, ETC. CAN FIRE EVENTS?

eventSubTasks, for now
-----
phrase
wait
autoMove thing (new moveType? Or refactor behaviors to be events?)


future
-----
unlock door/container
begin/end animation (add animation types: continuous, oneOff)
teleport thing
change scene

--]]