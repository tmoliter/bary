local Inventory = require('state.inventory')

local gameStateTemplate = {
    spawn = {},
    scenes = {},
    quests = {},
    inventories = {},
    party = {},
    -- status = {}
}

local gameState = {
    itemEventId = 0,
}

function gameState:fresh()
    for k, v in pairs(gameStateTemplate) do
        self[k] = deepcopy(v)
    end
end

local function splitPath(dotPath)
    local keys = {}
    for key in dotPath:gmatch("[^.]+") do
        keys[#keys + 1] = key
    end
    return keys
end

function gameState:updateQuest(dotPath, value)
    local keys = splitPath(dotPath)

    local node = self.quests
    for i = 1, #keys - 1 do
        local key = keys[i]
        if node[key] == nil then
            node[key] = {}
        end
        node = node[key]
    end

    node[keys[#keys]] = value
end

function gameState:updateQuests(quests)
    for quest,value in pairs(quests) do
        self:updateQuest(quest,value)
    end
end

function gameState:checkQuest(dotPath, value)
    local keys = splitPath(dotPath)

    local node = self.quests
    for i = 1, #keys - 1 do
        local key = keys[i]
        if node[key] == nil then
            return false
        end
        node = node[key]
    end

    return node[keys[#keys]] == value
end

function gameState:setSceneThing(sceneName, id, patch)
    local scene = self.scenes[sceneName]
    if scene == nil then
        scene = {}
        self.scenes[sceneName] = scene
    end
    if scene.things == nil then
        scene.things = {}
    end
    local existing = scene.things[id] or {}
    for k, v in pairs(patch) do
        existing[k] = v
    end
    scene.things[id] = existing
    return existing
end

function gameState:getSceneThing(sceneName, id)
    local scene = self.scenes[sceneName]
    if scene == nil or scene.things == nil then
        return nil
    end
    return scene.things[id]
end

function gameState:resolveSceneThings(sceneName, baseThings)
    local scene = self.scenes[sceneName] or {}
    local patches = scene.things or {}
    local removed = scene.removed or {}

    local resolved = {}
    for _, baseThing in ipairs(baseThings) do
        local id = baseThing.id
        if id == nil or not removed[id] then
            local thing = {}
            for k, v in pairs(baseThing) do thing[k] = v end
            if id ~= nil and patches[id] ~= nil then
                for k, v in pairs(patches[id]) do thing[k] = v end
            end
            resolved[#resolved + 1] = thing
        end
    end

    if scene.added ~= nil then
        for _, addedThing in pairs(scene.added) do
            local thing = {}
            for k, v in pairs(addedThing) do thing[k] = v end
            resolved[#resolved + 1] = thing
        end
    end

    return resolved
end

function gameState:addInventory(name, startingItems)
    self.inventories[name] = Inventory.new(
        startingItems and { items = startingItems } or {}
    )
end

function gameState:nextItemEventId()
    self.itemEventId = self.itemEventId + 1
    return self.itemEventId
end

gameState:fresh()

return gameState
