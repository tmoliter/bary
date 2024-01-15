local function sequentialTasks(hostThing, args)
    local tasks = args["tasks"]
    if args["pauseAllMoves"] then
        _newTask({{
            type = "pauseMoves",
            all = true,
        }}, args.eventName, hostThing)
        coroutine.yield()
    end
    for i,task in ipairs(tasks) do
        _newTask(tasks[i], args.eventName, hostThing)
        if i < #tasks then
            coroutine.yield()
        end
    end
    if args["pauseAllMoves"] then
        coroutine.yield()
        _newTask({{
            type = "pauseMoves",
            all = true,
            unpause = true
        }}, args.eventName, hostThing)
    end
end

local function randomAutoMove(hostThing, args)
    local originX, originY, variance, wait, stop = table.unpack {args["originX"], args["originY"], args["variance"], args["wait"], args["stop"]}
    local x, y
    local function move()
        x = math.random(originX - variance, originX + variance)
        y = math.random(originY - variance, originY + variance)
        _newTask({
            {
                type = "move",
                destinationX = x,
                destinationY = y
            },
        }, args.eventName, hostThing)
    end
    while stop ~= true do
        move()
        coroutine.yield()
        if wait ~= nil then
            _newTask(
                {
                    {
                        type = "wait",
                        frames = wait,
                    }
                }, args.eventName, hostThing
            )
            coroutine.yield()
        end
    end
end

local function openDoor(hostThing, args)
    -- in args we can define what is needed, if anything, to unlock the door, and
    -- in this function we will check inventory or quest status etc
    if (args["locked"]) then
        if args["catalyst"] == "interactable" then
            local phrase = args["lockedPhrase"] or {
                type = "phrase",
                text = "Locked.",
                x = 300,
                y = 150,
                width = 100,
                height = 30,
                scrollType = "continuous",
                gridLimitsX = 1000,
                gridLimitsY = 1000,
                blocking = true
            }
            _newTask(
                { phrase }, args.eventName, hostThing
            )
        end
        return
    end
    if args["catalyst"] == "trigger" and args["triggerDelay"] then
        _newTask({{
            type = "wait",
            frames = args["triggerDelay"],
        }}, args.eventName, hostThing)
        coroutine.yield()
    end
    _newTask({
        {
            type = "setActiveSprites",
            sprites =  { 0 }
        },
        {
            type = "disableColliders",
            obstructions = true
        }
    }, args.eventName, hostThing)
    if args["portal"] ~= nil then
        coroutine.yield()
        local portal = args["portal"]
        portal["thing"] = args["incomingThing"]
        portal["type"] = "portal"
        _newTask({portal}, args.eventName, hostThing)
    end
    if args["closeAfter"] ~= nil then
        coroutine.yield()
        _newTask({
            {
                type = "setActiveSprites",
                sprites = { 1 }
            },
            {
                type = "disableColliders",
                enable = true,
                obstructions = true
            }
    }, args.eventName, hostThing)
    end
end

local function inventoryMenu(hostThing, args)
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
    _newTask({
        {
            type = "pauseMoves",
            all = true,
        },
        {
            type = "menu",
            options = options,
            x = 300,
            y = 150,
            width = 340,
            height = 60,
            maxColumns = 2,
            closeOnDestroy = true,
            blocking = true
        }
    }, args.eventName, hostThing)
    local _, itemChoice = coroutine.yield()
    if itemChoice.selection == nil then 
        _newTask({{
            type = "pauseMoves",
            all = true,
            unpause = true
        }}, args.eventName, hostThing)
        return 
    end

    _newTask({
        {
            type = "menu",
            options = {
                {
                    selectionText = "use 1",
                    value = "use"
                },
                {
                    selectionText = "drop 1",
                    value = "drop"
                },
            },
            x = 350,
            y = 250,
            width = 80,
            height = 60,
            maxColumns = 1,
            closeOnDestroy = true,
            blocking = true
        }
    }, args.eventName, hostThing)

    local _, actionArgs = coroutine.yield()
    local action = actionArgs.selection

    ------------
    if action == "use" then
        local sufficient, remaining = gameState.inventories[args.inventoryName]:use(itemChoice.selection, 1, "jordan")
        if sufficient ~= true then
            print("INSUFFICIENT")
            return
        end
        local args = {
            eventDefinition = {
                type = "custom", 
                customCoroutine = itemDefinitions[itemChoice.selection].use,
                amount = 1,
                target = "jordan",
                source = args.inventoryName
            },
            eventName = "item_" .. gameState.itemEventId,
            catalyst = "item"
        }
        print("firing event")
        _newTask({
            {
                type = "pauseMoves",
                all = true,
                unpause = true
            },
            {
                type = "fireEvent",
                args = args
            }
        }, args.eventName, hostThing)
        return
    elseif action == "drop" then
        gameState.inventories[args.inventoryName]:drop(itemChoice.selection, 1, "jordan")
    end
    _newTask({{
        type = "pauseMoves",
        all = true,
        unpause = true
    }}, args.eventName, hostThing)
end

return {
    sequentialTasks = sequentialTasks,
    randomAutoMove = randomAutoMove,
    openDoor = openDoor,
    inventoryMenu = inventoryMenu
}
