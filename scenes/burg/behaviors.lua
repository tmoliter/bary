
local function zinniaAutoMove(originX, originY, hostThing)
    while true do
        _updateMoveTarget(originX - 150, originY - 50, hostThing)
        coroutine.yield()
        _updateMoveTarget(originX + 35, originY + 150, hostThing)
        coroutine.yield()
        _updateMoveTarget(originX, originY, hostThing)
        coroutine.yield()
    end
end

behaviors = {
    otherZinnia = {
        autoMove = coroutine.create(zinniaAutoMove)
    }
}

function doAutoMove(originX, originY, name, hostThing)
    if coroutine.status(behaviors[name]["autoMove"]) ~= 'dead' then
        coroutine.resume(behaviors[name]["autoMove"], originX, originY, hostThing) 
    end
end
