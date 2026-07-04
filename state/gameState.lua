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

function gameState:addInventory(name, startingItems)
    self.inventories[name] = Inventory.new(
        startingItems and { items = startingItems } or {}
    )
end

function gameState:nextItemEventId()
    self.itemEventId = self.itemEventId + 1
    return self.itemEventId
end

function gameState:fresh()
    for k, v in pairs(gameStateTemplate) do
        self[k] = deepcopy(v)
    end
end

gameState:fresh()

return gameState
