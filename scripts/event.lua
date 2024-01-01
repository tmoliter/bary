local standardEvents = require('scripts.standardEvents')

local activeEvents = {}
local eventDefinitions = {}

-- maybe make a bulk beginEvents function here

local function beginEvent(hostThing, args)
    local thingName, eventName = table.unpack { args["thingName"], args["eventName"] }

    local eventDefinition = eventDefinitions[thingName][eventName]

    -- Event has never been invoked
    if activeEvents[hostThing] == nil then
        activeEvents[hostThing] = {}
    end
    if activeEvents[hostThing][eventName] == nil then
        activeEvents[hostThing][eventName] = { timesInvoked = 0 }
    end

    local activeEvent = activeEvents[hostThing][eventName]
    
    -- Event is already in progress
    if activeEvent["coroutine"] ~= nil then
        return
    end


    activeEvent["args"] = {}
    for k,v in pairs(eventDefinition) do activeEvent["args"][k] = v end
    for k,v in pairs(args) do activeEvent["args"][k] = v end

    if eventDefinition["type"] == "custom" then
        activeEvent["coroutine"] = coroutine.create(eventDefinition["customCoroutine"], hostThing, activeEvent["args"], eventName)
    else
        activeEvent["coroutine"] = coroutine.create(standardEvents[eventDefinition["type"]], hostThing, activeEvent["args"], eventName)
    end

    activeEvent["timesInvoked"] = activeEvent["timesInvoked"] + 1

    -- return task data for C++ here?
    -- Data will be an event name, a list of subtasks, and a table of data for each subtask
    resumeEvent(hostThing, eventName)
end


local function resumeEvent(hostThing, eventName)
    -- Event is not active
    if activeEvents[hostThing] == nil or
        activeEvents[hostThing][eventName] == nil or
        activeEvents[hostThing][eventName]["coroutine"] == nil
        then
        return 0
    end

    local activeEvent = activeEvents[hostThing][eventName]

    coroutine.resume(activeEvent["coroutine"], hostThing, activeEvent["args"], eventName)
    if coroutine.status(activeEvent["coroutine"]) == 'dead' then
        activeEvent["coroutine"] = nil
        activeEvent["args"] = nil
        return 0
    end
    return 1
end

local function populateDefinitions(allThings, definitions)
    for _,thing in pairs(allThings) do
        eventDefinitions[thing["name"]] = thing["events"]
    end
end

return {
    beginEvent,
    resumeEvent,
    populateDefinitions
}

--[[

DO WE NEED TO DECOUPLE EVENTS FROM COLLIDABLES EARLY, SO THAT THINGS LIKE BUTTON PRESSES, ETC. CAN FIRE EVENTS?

eventSubTasks, for now
-----
phrase
wait
autoMove thing


future
-----
unlock door/container
begin/end animation (add animation types: continuous, oneOff)
teleport thing
change scene

--]]