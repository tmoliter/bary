local function randomAutoMove(hostThing, args)
    originX, originY, variance, stop = table.unpack {args["originX"], args["originY"], args["variance"], args["stop"]}
    function move()
        local x = math.random(originX - variance, originX + variance)
        local y = math.random(originY - variance, originY + variance)
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
        _newTask(
            {
                {
                    type = "wait",
                    frames = 150,
                    -- blocking = true
                }
            }, args["eventName"], hostThing
        )
        coroutine.yield()
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


return {
    randomAutoMove = randomAutoMove,
    simpleMessages = simpleMessages
}
