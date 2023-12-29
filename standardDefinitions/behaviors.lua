local function randomAutoMove(hostThing, args)
    originX, originY, variance, stop = table.unpack {args["originX"], args["originY"], args["variance"], args["stop"]}
    local function move()
        local x = math.random(originX - variance, originX + variance)
        local y = math.random(originY - variance, originY + variance)
        _updateMoveTarget(x, y, hostThing)
    end
    while stop ~= true do
        move()
        coroutine.yield()
        _newTask(
            {
                {
                    type = "wait",
                    frames = 50,
                    -- blocking = true
                }
            }, "", hostThing
        )
        coroutine.yield()
        -- move()
        -- _newTask(
        --     {
        --         {
        --             type = "phrase",
        --             text = "continue?",
        --             x = 300,
        --             y = 100,
        --             width = 100,
        --             height = 80,
        --             scrollType = "continuous",
        --             gridLimitsX = 1000,
        --             gridLimitsY = 1000,
        --         }
        --     }, "", hostThing
        -- )
        -- coroutine.yield()
    end
end

return {
    randomAutoMove = randomAutoMove
}
