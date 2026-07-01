local activeEvents = {}
local eventDefinitions = {}
local collidableEvents = {}

-- maybe make a bulk beginEvents function here

local function beginEvent(hostThing, args)
    local eventDefinition
    if args["eventDefinition"] ~= nil then
        eventDefinition = args["eventDefinition"]
    else
        eventDefinition = eventDefinitions[args["thingName"]][args.eventName]
    end

    -- No events have been invoked for this thing?
    if activeEvents[hostThing] == nil then
        activeEvents[hostThing] = {}
    end
    -- Event has never been invoked
    if activeEvents[hostThing][args.eventName] == nil then
        activeEvents[hostThing][args.eventName] = { timesInvoked = 0 }
    end

    local activeEvent = activeEvents[hostThing][args.eventName]

    -- Event is already in progress
    if activeEvent["coroutine"] ~= nil then
        return
    end


    activeEvent["args"] = { gameState = gameState }
    for k,v in pairs(eventDefinition) do activeEvent["args"][k] = v end
    for k,v in pairs(args) do activeEvent["args"][k] = v end

    if eventDefinition["type"] == "custom" then
        activeEvent["coroutine"] = coroutine.create(eventDefinition["customCoroutine"], hostThing, activeEvent["args"])
    elseif eventDefinition["type"] ~= nil then
        activeEvent["coroutine"] = coroutine.create(standardEvents[eventDefinition["type"]], hostThing, activeEvent["args"])
    else -- item case
        activeEvent["coroutine"] = coroutine.create(eventDefinition, hostThing, activeEvent["args"])
    end

    activeEvent["timesInvoked"] = activeEvent["timesInvoked"] + 1

    -- return task data for C++ here?
    -- Data will be an event name, a list of subtasks, and a table of data for each subtask
    resumeEvent(hostThing, activeEvent["args"])
end


local function resumeEvent(hostThing, args)
    -- Event is not active
    if activeEvents[hostThing] == nil or
        activeEvents[hostThing][args.eventName] == nil or
        activeEvents[hostThing][args.eventName]["coroutine"] == nil
        then
        return 0
    end

    local activeEvent = activeEvents[hostThing][args.eventName]

    coroutine.resume(activeEvent["coroutine"], hostThing, args)
    if coroutine.status(activeEvent["coroutine"]) == 'dead' then
        activeEvent["coroutine"] = nil
        activeEvent["args"] = nil
        return 0
    end
    return 1
end

local function fireCollidable(hostThing, args)
    local bindings = collidableEvents[args["thingName"]]
    if bindings == nil then return end
    local eventNames = bindings[args["collidableName"]]
    if eventNames == nil then return end
    for _,eventName in ipairs(eventNames) do
        args.eventName = eventName
        beginEvent(hostThing, args)
    end
end

local function clearAllEvents(hostThing)
    activeEvents = {}
end

local standardColliderNames = { interactable = "standardInteract", trigger = "standardTrigger" }
local function bindCollidableEvents(thing)
    if thing["components"] == nil then return end
    for _,component in pairs(thing["components"]) do
        if component["type"] == "standardCollider" and component["eventNames"] ~= nil then
            for catalyst,collidableName in pairs(standardColliderNames) do
                if component[catalyst] then
                    if collidableEvents[thing["name"]] == nil then collidableEvents[thing["name"]] = {} end
                    collidableEvents[thing["name"]][collidableName] = component["eventNames"]
                end
            end
        end
    end
end

local function populateEvents(thing)
    if eventDefinitions[thing["name"]] == nil then eventDefinitions[thing["name"]] = thing["events"] end
    bindCollidableEvents(thing)
    if thing["subThings"] ~= nil then
        for _,subThing in pairs(thing["subThings"]) do
            populateEvents(subThing)
        end
    end
end

local function populate(thingDefs, sceneEvents)
    eventDefinitions = {}
    collidableEvents = {}
    for _,thing in pairs(thingDefs) do
        populateEvents(thing)
    end
    eventDefinitions.gameManager = sceneEvents
end

return {
    beginEvent = beginEvent,
    resumeEvent = resumeEvent,
    fireCollidable = fireCollidable,
    populate = populate,
    clearAllEvents = clearAllEvents
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