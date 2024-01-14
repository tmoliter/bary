local standardEvents = require('scripts.standardEvents')
local itemDefinitions = require(settings.GAME_NAME .. '.itemDefinitions')
local activeEvents = {}
local eventDefinitions = {}
local itemEventId = 1

-- maybe make a bulk beginEvents function here

local function useItem(source, target, itemName, amount)
    local sufficient, remaining = gameState.inventories[source]:use(itemName, amount, target)
    if sufficient ~= true then
        return
    end
    local args = {
        eventDefinition = { 
            type = "custom", 
            customCoroutine = itemDefinitions[itemName].use,
            amount = amount,
            itemName = itemName,
            target = target,
            source = source
        },
        eventName = "item_" .. itemEventId,
        catalyst = "item"
    }
    itemEventId = itemEventId + 1
    beginEvent(sceneManager, args)
end

local function beginEvent(hostThing, args)
    local eventDefinition
    if args["eventDefinition"] ~= nil then
        eventDefinition = args["eventDefinition"]
        itemEventId = itemEventId + 1
    else
        eventDefinition = eventDefinitions[args["thingName"]][args.eventName]
    end

    -- Event has never been invoked
    if activeEvents[hostThing] == nil then
        activeEvents[hostThing] = {}
    end
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

local function populateEvents(thing)
    -- we might do additional stuff with components here
    if eventDefinitions[thing["name"]] == nil then eventDefinitions[thing["name"]] = thing["events"] end
    if thing["subThings"] ~= nil then
        for _,subThing in pairs(thing["subThings"]) do
            populateEvents(subThing)
        end
    end
end

local function populate(thingDefs) 
    eventDefinitions = {}
    for _,thing in pairs(thingDefs) do
        populateEvents(thing)
    end
end

return {
    useItem = useItem,
    beginEvent = beginEvent,
    resumeEvent = resumeEvent,
    populate = populate
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