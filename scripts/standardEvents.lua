local function randomAutoMove(hostThing, args)
    local originX, originY, variance, wait, stop = table.unpack {args["originX"], args["originY"], args["variance"], args["wait"], args["stop"]}
    local x, y
    local function move()
        x = math.random(originX - variance, originX + variance)
        y = math.random(originY - variance, originY + variance)
        _newTask({
            {
                type = "move",
                auto = true,
                destinationX = x,
                destinationY = y
            },
        }, args["eventName"], hostThing)
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
                }, args["eventName"], hostThing
            )
            coroutine.yield()
        end
    end
end

local function simpleMessages(hostThing, args, eventName)
    local index = 1
    for k,v in ipairs(args["phrases"]) do
        v["type"] = "phrase"
        v["blocking"] = true
        _newTask({ v }, eventName, hostThing)
        if index < #args["phrases"] then
            index = index + 1
            coroutine.yield()
        end
    end
end

local function openDoor(hostThing, args, eventName)
    -- in args we can define what is needed, if anything, to unlock the door, and
    -- in this function we will check inventory or quest status etc
    if (args["locked"]) then
        if args["collisionType"] == "interactable" then
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
                { phrase }, eventName, hostThing
            )
        end
        return
    end
    if args["collisionType"] == "trigger" and args["triggerDelay"] then
        _newTask({{
            type = "wait",
            frames = args["triggerDelay"],
        }}, eventName, hostThing)
        coroutine.yield()
    end
    _newTask({
        {
            type = "setActiveSprites",
            sprites =  { 0 }
        },
        {
            type = "disableColliders",
            all = true -- should only disable the obstruction
        }
    }, eventName, hostThing)
    if args["portal"] ~= nil then
        coroutine.yield()
        local portal = args["portal"]
        portal["thing"] = args["incomingThing"]
        portal["type"] = "portal"
        _newTask({portal}, eventName, hostThing)
    end
    if args["closeAfter"] ~= nil then
        coroutine.yield()
        _newTask({{
            type = "setActiveSprites",
            sprites = { 1 }
        }}, eventName, hostThing)
    end
end



return {
    randomAutoMove = randomAutoMove,
    simpleMessages = simpleMessages,
    openDoor = openDoor
}
