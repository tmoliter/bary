local Inventory = require('state.inventory')

local GameState = {
    addInventory = function(self, name, startingItems)
        self.inventories[name] = Inventory.new(
            startingItems and { items = startingItems } or {}
        ) 
    end
}

local gameState = {
    -- spawn = {},
    -- scenes = {},
    -- quests = {},
    inventories = {},
    -- status = {}
}
setmetatable(gameState, {__index = GameState})
Inventory.populateGameState(gameState)

return gameState