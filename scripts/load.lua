function loadScene(host, sceneName)
    require('scenes.' .. sceneName .. '.behaviors')

    local mapTable = require('scenes.' .. sceneName .. '.map')
    _loadScene(mapTable["backgroundPath"], mapTable["allThings"], host)
    
    local setup = require('scenes.' .. sceneName .. '.setup')
    setup(host)
end
