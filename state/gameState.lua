local Inventory = require('state.inventory')
local GameState = {
    addInventory = function(self, name, startingItems)
        self.inventories[name] = Inventory.new(
            startingItems and { items = startingItems } or {}
        )
    end,
    itemEventId = 0
}

local gameStateTemplate = {
    spawn = {},
    scenes = {},
    -- quests = {},
    inventories = {},
    party = {},
    -- status = {}
}
GameState.fresh = function(self)
    self = deepcopy(gameStateTemplate)
end

local gameState = deepcopy(gameStateTemplate)
setmetatable(gameState, {
        __index = function(t,k)
            if k == "itemEventId" then
                GameState.itemEventId = GameState.itemEventId + 1;
            end
            return GameState[k]
        end
    }
)

return gameState