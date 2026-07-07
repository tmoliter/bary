local bundles = {}

local function expandOpenable(thingDef)
    if thingDef.subThings ~= nil then
        for _,subThing in ipairs(thingDef.subThings) do
            expandOpenable(subThing)
        end
    end

    local openable = thingDef.openable
    if openable == nil or thingDef._openableExpanded then
        return
    end

    thingDef._openableExpanded = true

    local openEvent = {
        type = "open",
        persist = openable.persist,
        triggerDelay = openable.triggerDelay,
        closeAfter = openable.closeAfter,
        locked = openable.locked,
        openSprites = openable.sprites and openable.sprites.open,
        closeSprites = openable.sprites and openable.sprites.closed,
        disableCollidables = openable.collidablesWhenOpen ~= true,
        disableObstructions = openable.obstructionsWhenOpen == false,
    }
    if openable.onOpen ~= nil then
        for k,v in pairs(openable.onOpen) do openEvent[k] = v end
    end
    thingDef.events = thingDef.events or {}
    thingDef.events.open = openEvent

    if thingDef.interactableData == nil and thingDef.triggerData == nil then
        local catalyst = openable.catalyst or "interactable"
        local catalysts = type(catalyst) == "table" and catalyst or { catalyst }
        local interactable, trigger = false, false
        for _,c in ipairs(catalysts) do
            if c == "interactable" then interactable = true end
            if c == "trigger" then trigger = true end
        end
        thingDef.components = thingDef.components or {}
        thingDef.components[#thingDef.components + 1] = {
            type = "standardCollider",
            interactable = interactable,
            trigger = trigger,
            eventNames = { "open" },
        }
    end
end

function bundles.expandOpenableDefs(thingDefs)
    for _,thingDef in pairs(thingDefs) do
        expandOpenable(thingDef)
    end
end

function bundles.applyOpenable(spawn)
    local openable = spawn.openable
    if openable == nil then
        return
    end
    local state = spawn.openState or openable.initial or "closed"
    spawn.openState = state

    if openable.sprites ~= nil and spawn.spriteDataVector ~= nil then
        local activeSet = {}
        for _,zeroIndex in ipairs(openable.sprites[state] or {}) do
            activeSet[zeroIndex] = true
        end
        local sprites = deepcopy(spawn.spriteDataVector)
        for i,sprite in ipairs(sprites) do
            sprite.active = activeSet[i - 1] == true
        end
        spawn.spriteDataVector = sprites
    end

    if state == "open" then
        local keepObstruction = openable.obstructionsWhenOpen ~= false
        local keepCollidable = openable.collidablesWhenOpen == true

        if spawn.components ~= nil and not keepCollidable then
            local components = {}
            for _,component in ipairs(spawn.components) do
                if component.type ~= "standardCollider" then
                    components[#components + 1] = component
                elseif keepObstruction then
                    local c = deepcopy(component)
                    c.interactable = false
                    c.trigger = false
                    components[#components + 1] = c
                end
            end
            spawn.components = components
        end

        if not keepCollidable then
            spawn.interactableData = nil
            spawn.triggerData = nil
        end

        if not keepObstruction then
            spawn.obstructionData = {}
        end
    end
end

local function collisionTask(args, enable)
    if not (args.disableCollidables or args.disableObstructions) then
        return nil
    end
    return {
        type = "disableColliders",
        enable = enable,
        interactables = args.disableCollidables == true,
        triggers = args.disableCollidables == true,
        obstructions = args.disableObstructions == true,
    }
end

function bundles.buildOpenTask(args)
    local tasks = {{ type = "setActiveSprites", sprites = args.openSprites or { 0 } }}
    local toggle = collisionTask(args, false)
    if toggle then tasks[#tasks + 1] = toggle end
    return tasks
end

function bundles.buildCloseTask(args)
    local tasks = {{ type = "setActiveSprites", sprites = args.closeSprites or { 1 } }}
    local toggle = collisionTask(args, true)
    if toggle then tasks[#tasks + 1] = toggle end
    return tasks
end

return bundles
