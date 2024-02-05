local Resources = {
    prototype = {
        resources = {},
        getTextures = function(self)
            local t = {}
            for k,v in pairs(self.ownTextures or {}) do t[k] = "games/" .. settings.GAME_NAME .. "/assets/" .. v .. ".png" end
            for k,v in pairs(self.baseTextures or {})do t[k] = "base/assets/" .. v .. ".png" end
            return t
        end
    },
    mt = {}
}
Resources.mt.__index = function(t,k) return Resources.prototype[k] end
Resources.new = function(o) setmetatable(o, Resources.mt); return o end

return Resources