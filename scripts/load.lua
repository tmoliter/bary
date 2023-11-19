function loadScene(sceneName, host)
    local mapTable = require('scenes.' .. sceneName .. '.map')
    _loadScene(mapTable["backgroundPath"], mapTable["allThings"], host)
    local setup = require('scenes.' .. sceneName .. '.setup')
    setup(host)
end
