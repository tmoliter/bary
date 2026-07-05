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
        openable = openable,
        triggerDelay = openable.triggerDelay,
        closeAfter = openable.closeAfter,
        locked = openable.locked,
    }
    if openable.onOpen ~= nil then
        for k,v in pairs(openable.onOpen) do openEvent[k] = v end
    end
    thingDef.events = thingDef.events or {}
    thingDef.events.open = openEvent

    local catalyst = openable.catalyst or "interactable"
    thingDef.components = thingDef.components or {}
    thingDef.components[#thingDef.components + 1] = {
        type = "standardCollider",
        interactable = catalyst == "interactable",
        trigger = catalyst == "trigger",
        eventNames = { "open" },
    }
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

        if not keepObstruction then
            spawn.obstructionData = {}
        end
    end
end

function bundles.handleOpen(openable)
    local tasks = {{
        type = "setActiveSprites",
        sprites = (openable.sprites and openable.sprites.open) or { 0 }
    }}
    local disableCollidables = openable.collidablesWhenOpen ~= true
    local disableObstructions = openable.obstructionsWhenOpen == false
    if disableCollidables or disableObstructions then
        tasks[#tasks + 1] = {
            type = "disableColliders",
            interactables = disableCollidables,
            triggers = disableCollidables,
            obstructions = disableObstructions,
        }
    end
    return tasks
end

return bundles
