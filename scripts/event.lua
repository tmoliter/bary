local activeEvents = {}
local activeBehaviors = {}
local eventDefinitions = {}

local function resumeEvent(hostScene, hostThing, eventName)
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
    coroutine.resume(activeEvent["coroutine"], hostScene, hostThing, activeEvent["args"], eventName)
    if coroutine.status(activeEvent["coroutine"]) == 'dead' then
        activeEvent["coroutine"] = nil
        return true
    end
    return false
end


local function beginEvent(hostScene, hostThing, thingName, collidableName, args)
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
            activeEvent["coroutine"] = coroutine.create(eventDefinition["customCoroutine"], hostScene, hostThing, activeEvent["args"])
        elseif eventDefinition["type"] == "simpleMessages" then
            activeEvent["coroutine"] = coroutine.create(simpleMessages, hostScene, hostThing, activeEvent["args"])
        end
        activeEvent["timesInvoked"] = activeEvent["timesInvoked"] + 1

        -- return task data for C++ here?
        -- Data will be an event name, a list of subtasks, and a table of data for each subtask
        resumeEvent(hostScene, hostThing, eventName)
    end
end

local function simpleMessages(hostScene, hostThing, args, eventName)
    local index = 1
    for k,v in ipairs(args["phrases"]) do
        _newTask(v, { "phrase" }, eventName, hostThing, hostScene)
        if index < #args["phrases"] then
            index = index + 1
            coroutine.yield()
        end
    end
end


local function beginAutoMove(hostScene, hostThing, originX, originY, hostThingName)
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

local function doAutoMove(hostScene, hostThing)
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
end

local function populateDefintions(e)
    eventDefinitions = e
end

return {
    resumeEvent,
    beginEvent,
    simpleMessages,
    beginAutoMove,
    doAutoMove,
    populateDefintions
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


each eventTask can have multiple SubTasks, and only reports back to Lua when none are left?

might need to call C++ with a list of subTask types followed by tables of arguments for each..
we'd probably want to validate the contents of tables as well with a new table validation function

C++ EVENT TASK: has a regular, incrementing ID, along with pointer to RealThing and Scene, and vector of SubTasks.
Upon construction, the Event Task calls Lua, where the ID becomes a key in the events {} table
The value of the table contains a reference to the coroutine and other stuff.
Coroutine serves to call C++ with a list of SubTasks and tables with arguments for those SubTasks.
These populate the Task on construction, and each time the event exhausts its SubTasks, until the coroutine dies and the Task terminates
Scene has a regular vector of Tasks, and if it is not empty we loop through Tasks before meatThings.
If any Tasks are blockMeat = true, then we skip meatThings. Allowing only animation or only movement and animation are also possible.
We only give live input to the the Task at .back() of the event task vector
Inside the Task's meat call, we loop through SubTasks and call their meat.
Once all SubTasks have terminated themselves, the Event Task calls lua to resume the coroutine


12/9 There might be events that live in a neutral directory and get loaded into every scene, 
     like inventory menu event, etc.

--]]