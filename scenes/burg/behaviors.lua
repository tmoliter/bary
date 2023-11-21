
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

local function zinniaTalk(hostThing)
    _updateMoveSpeed(0, hostThing)
    _phrase("Hey this thing's working", "talk", hostThing)
    coroutine.yield()
    _phrase("Alright, this is goodbye", "talk", hostThing)
    coroutine.yield()
    _updateMoveSpeed(1, hostThing)
end

behaviors = {
    otherZinnia = {
        autoMove = coroutine.create(zinniaAutoMove)
    }
}

events = {
    otherZinnia = {
        talk = coroutine.create(zinniaTalk)
    }
}

 -- NOTE: These sorts of functions should be moved somewhere scene agnostic and re-usable

function doAutoMove(originX, originY, name, hostThing)
    if coroutine.status(behaviors[name]["autoMove"]) ~= 'dead' then
        coroutine.resume(behaviors[name]["autoMove"], originX, originY, hostThing) 
    end
end

function resumeEvent(eventName, thingName, hostThing)
    if coroutine.status(events[thingName][eventName]) == 'dead' then
        return 1
    end
    coroutine.resume(events[thingName][eventName], hostThing)
    return 0
end