function loadScene(host, sceneName)
    eventModule = require("scripts.event")
    resumeEvent, beginEvent, simpleMessages, beginAutoMove, doAutoMove, populateDefintions = table.unpack(eventModule)
    populateDefintions(require('scenes.' .. sceneName .. '.behaviors'))

    local mapTable = require('scenes.' .. sceneName .. '.map')
    _loadScene(mapTable["backgroundPath"], mapTable["allThings"], host)
    
    local setup = require('scenes.' .. sceneName .. '.setup')
    setup(host)
end
