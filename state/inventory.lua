local Inventory = {
    prototype = {
        count = function(self, itemName)
            return self.items[itemName]
        end,
        add = function(self, itemName, amount)
            self.items[itemName] = self.items[itemName] + amount
            return self.items[itemName]
        end,
        drop = function(self, itemName, amount)
            if self.items[itemName] - amount < 0 then
                self.items[itemName] = nil
                return false, 0
            end
            self.items[itemName] = self.items[itemName] - amount
            if self.items[itemName] < 1 then self.items[itemName] = nil end
            return true, self.items[itemName]
        end,
        use = function(self, itemName, amount, target)
            if self.items[itemName] < amount then
                print("OUT OF " .. itemName)
                return false, amount end
            print("used " .. amount .. "/" .. self.items[itemName] .. " " .. itemName)
            return self:drop(itemName, amount)
        end,
    },
    mt = {}
}
Inventory.mt.__index = function(t,k) return Inventory.prototype[k] end
Inventory.new = function(o)
    o.items = o.items or {}
    setmetatable(o.items, {
        __index = function(t,k) 
            return 0
        end
    })
    setmetatable(o, Inventory.mt)
    return o
end

return Inventory