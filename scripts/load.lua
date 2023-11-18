function loadScene(scene, host)
    local mapTable = require('scenes.' .. scene .. ".map")
    _loadScene(mapTable["backgroundPath"], mapTable["allThings"], host)
end
