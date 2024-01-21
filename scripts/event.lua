local activeEvents = {}
local eventDefinitions = {}

-- maybe make a bulk beginEvents function here

local function beginEvent(hostThing, args)
    local eventDefinition
    if args["eventDefinition"] ~= nil then
        eventDefinition = args["eventDefinition"]
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
    eventDefinitions.sceneManager = {
        inventoryMenu = {
            type = "inventoryMenu"
        }
        NEWinventoryMenu = {
            type = "menu",
            x = 300,
            y = 150,
            width = 340,
            height = 60,
            maxColumns = 2,
            blocking = true,
            getContents = function(args) 
                local function nameAndAmount (itemDef, amount)
                    if amount == 1 then
                        return amount .. " " .. itemDef.name
                    end
                    return  amount .. " " .. itemDef.plural
                end
                local options = {}
                local i = 1
                for itemKey, amount in pairs(gameState.inventories[args.inventoryName].items) do
                    options[i] = { 
                        selectionText = nameAndAmount(itemDefinitions[itemKey], amount), 
                        value = itemKey
                    }
                    i = i+1
                end
                return options
            end
            onSelect = function(hostThing, invMenuArgs, invMenuSelectionArgs)
                if invMenuSelectionArgs.selection == "" then 
                    invMenuSelectionArgs:close() 
                    return
                end
                invMenuSelectionArgs.loseControl = true
                _newTask({{
                    type = "fireEvent",
                    args = {
                        eventName = "itemActionMenu",
                        catalyst = "menu",
                        eventDefinition = {
                            type = "menu",
                            allowCancel = true,
                            x = 350,
                            y = 250,
                            width = 80,
                            height = 60,
                            maxColumns = 1,
                            blocking = true,
                            itemSelection = invMenuSelectArgs["selection"],
                            parentRef = invMenuArgs["menu"]
                            getContents = function(actionSelectArgs)
                                return {
                                    {
                                        selectionText = "use",
                                        value = "use"
                                    },
                                    {
                                        selectionText = "drop",
                                        value = "drop"
                                    }
                                }
                            end,
                            onSelect = function(hostThing, actionMenuArgs, actionSelectionArgs)
                                if actionSelectionArgs.selection == "" then
                                    _newTask({
                                        type = "menu",
                                        menu = parentRef,
                                    }, "inventoryMenu", hostThing)
                                    actionSelectionArgs:close()
                                    return
                                end
                                if itemDefinitions[actionSelectionArgs.itemSelection].needsTarget then
                                    actionSelectionArgs.loseControl = true
                                    _newTask({{
                                        type = "fireEvent",
                                        args = {
                                            eventName = "itemTargetMenu",
                                            catalyst = "menu",
                                            eventDefinition = {
                                                type = "menu",
                                                allowCancel = true,
                                                x = 480,
                                                y = 250,
                                                width = 80,
                                                height = 60,
                                                maxColumns = 1,
                                                blocking = true,
                                                itemSelection = actionArgs.itemSelection,
                                                targetWrapper = targetWrapper,
                                                grandParentRef = actionMenuArgs.parentRef,
                                                parentRef = actionSelectionArgs.menu,
                                                use = actionSelectionArgs.use,
                                                getContents = function(targetArgs)
                                                    local options = {}
                                                    for _,partyMemberName in ipairs(gameState.party) do 
                                                        table.insert(options, {
                                                        selectionText = partyMemberName,
                                                        value = partyMemberName
                                                        })
                                                    end
                                                    return options
                                                end,
                                                onSelect = function(hostThing, targetArgs, targetSelectArgs)
                                                    if targetSelectArgs.selection == "" then 
                                                        _newTask({
                                                            type = "menu",
                                                            menu = parentRef,
                                                        }, "itemActionMenu", hostThing)
                                                        targetArgs:close() 
                                                        return
                                                    end
                                                    use(targetSelectArgs:(targetSelectArgs.selection))
                                                    _newTask({
                                                        type = "menu",
                                                        menu = grandParentRef,
                                                        close = true,
                                                    }, "inventoryMenu", hostThing)
                                                    _newTask({
                                                        type = "menu",
                                                        menu = parentRef,
                                                        close = true,
                                                    }, "itemActionMenu", hostThing)
                                                    targetSelectArgs:close()
                                                end
                                            }
                                        }
                                    }}, "itemTargetMenu", hostThing)
                                end
                            end
                        }
                    }
                }}, "itemActionMenu", hostThing)
            end
        }
    }
end

return {
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