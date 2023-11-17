function loadMap(scene)
    local mapTable = require('scenes.' .. scene .. ".map")
    _G.allThings = mapTable["allThings"]
    _G.backgroundPath = mapTable["backgroundPath"]
end
