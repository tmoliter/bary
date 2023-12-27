function loadScene(host, sceneName)
    eventModule = require("scripts.event")
    resumeEvent, beginEvent, simpleMessages, beginBehavior, doBehavior, populateDefintions = table.unpack(eventModule)
    populateDefintions(require('scenes.' .. sceneName .. '.definitions'))

    local mapTable = require('scenes.' .. sceneName .. '.map')
    _loadScene(mapTable["backgroundPath"], mapTable["allThings"], host)
    
    local setup = require('scenes.' .. sceneName .. '.setup')
    setup(host)
end
