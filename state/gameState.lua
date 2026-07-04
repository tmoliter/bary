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

function gameState:updateQuest(dotPath, value)
    local keys = {}
    for key in dotPath:gmatch("[^.]+") do
        keys[#keys + 1] = key
    end

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

function gameState:checkQuest(dotPath, value)
    local keys = {}
    for key in dotPath:gmatch("[^.]+") do
        keys[#keys + 1] = key
    end

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
