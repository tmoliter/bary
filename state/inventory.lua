local itemDefinitions = {
    mungBeanJuice = {
        use = function(amount, target, gameState)
            print("USING " .. amount .. " MUNG BEAN JUICES ON " .. target)
            gameState.inventories[target]:add("oolong", amount * 2)
            print(target .. " NOW HAS " .. gameState.inventories[target]:count("oolong") .. " OOLONGS")
        end
    },
    oolong = {
        use = function(amount, target, gameState)
            print("used " .. amount .. " oolong")
        end
    }
}

setmetatable(itemDefinitions, {
    __index = function(t,k)
        print("No item definition for " .. k .. "found!")
        error()
    end
})

local Inventory = {
    prototype = {
        gameState = nil,
        itemDefinitions = itemDefinitions,
        count = function(self, itemName)
            if self.itemDefinitions[itemName] then end -- null check
            return self.items[itemName]
        end,
        add = function(self, itemName, amount)
            if self.itemDefinitions[itemName] then end -- null check
            self.items[itemName] = self.items[itemName] + amount
            return self.items[itemName]
        end,
        drop = function(self, itemName, amount)
            if self.itemDefinitions[itemName] then end -- null check
            if self.items[itemName] == 0 then
                return 0 end
            self.items[itemName] = self.items[itemName] - amount
            return self.items[itemName]
        end,
        use = function(self, itemName, amount, target)
            if self.itemDefinitions[itemName] then end -- null check
            if self.items[itemName] < amount then
                print("OUT OF " .. itemName)
                return 0 end
            self.itemDefinitions[itemName].use(amount, target, self.gameState)
            return self:drop(itemName, amount)
        end,
    },
    mt = {}
}
Inventory.populateGameState = function(gameState) Inventory.prototype.gameState = gameState end
Inventory.mt.__index = function(t,k) return Inventory.prototype[k] end
Inventory.new = function(o)
    if Inventory.prototype.gameState == nil then
        print("must initialize Inventory prototype")
        error()
    end
    o.items = o.items or {}
    setmetatable(o.items, {
        __index = function(t,k) 
            t[k] = 0
            return t[k]
        end
    })
    setmetatable(o, Inventory.mt)
    return o
end

return Inventory