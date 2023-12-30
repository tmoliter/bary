function loadScene(host, sceneName)
    local eventModule = require("scripts.event")
    local mapTable = require('scenes.' .. sceneName .. '.map')
    local setup = require('scenes.' .. sceneName .. '.setup')

    local populateDefinitions
    local allThings = mapTable["allThings"]

    beginEvent, resumeEvent, populateDefinitions = table.unpack(eventModule)
    local customThings, eventDefinitions = table.unpack(setup)
    
    for _,thing in ipairs(customThings) do table.insert(allThings, thing) end
    populateDefinitions(allThings, eventDefinitions)

    _loadScene(mapTable["backgroundPath"], allThings, host)
end
