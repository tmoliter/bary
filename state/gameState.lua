local Inventory = require('state.inventory')
local GameState = {
    addInventory = function(self, name, startingItems)
        self.inventories[name] = Inventory.new(
            startingItems and { items = startingItems } or {}
        )
    
    end
}

local gameStateTemplate = {
    spawn = {},
    scenes = {},
    -- quests = {},
    inventories = {},
    -- party = {},
    -- status = {}
}
GameState.fresh = function(self)
    self = deepcopy(gameStateTemplate)
end

local gameState = deepcopy(gameStateTemplate)
setmetatable(gameState, {__index = GameState})

return gameState