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

local function menu(hostThing, args)
    local taskArgs = deepcopy(args)
    taskArgs.options = args.getContents(args)
    _newTask({
        {
            type = "menu",
            options = args.getContents(args),
            x = 300,
            y = 150,
            width = 340,
            height = 60,
            maxColumns = 2,
            blocking = true
        },
        {
            type = "pauseMoves",
            all = true,
        }
    }, args.eventName, hostThing)

    local close = function(self)
        self.menu = nil
    end

    while true do
        local _, selectionArgs = coroutine.yield()
        selectionArgs.close = close
        onSelect(hostThing, args, selectionArgs)

        if not selectionArgs.menu then break end
        _newTask({
            type = "menu",
            menu = selectionArgs.menu
        }, args.eventName, hostThing)
    end

    _newTask({
        {
            type = "menu",
            menu = selectionArgs.menu,
            close = true,
        },
        {
            type = "pauseMoves",
            all = true,
            unpause = true
        }
    }, args.eventName, hostThing)
    return
end

return {
    sequentialTasks = sequentialTasks,
    randomAutoMove = randomAutoMove,
    openDoor = openDoor,
    menu = menu,
}
