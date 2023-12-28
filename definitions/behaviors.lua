local function randomAutoMove(hostThing, args)
    originX, originY, variance, stop = table.unpack {args["originX"], args["originY"], args["variance"], args["stop"]}
    while stop ~= true do
        local x = math.random(originX - variance, originX + variance)
        local y = math.random(originY - variance, originY + variance)
        _updateMoveTarget(x, y, hostThing)
        coroutine.yield()
    end
end

return {
    randomAutoMove
}