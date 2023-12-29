function loadScene(host, sceneName)
    local eventModule = require("scripts.event")
    local populateDefinitions
    resumeEvent, beginEvent, simpleMessages, beginBehavior, doBehavior, populateDefinitions = table.unpack(eventModule)
    populateDefinitions(require('scenes.' .. sceneName .. '.definitions'))

    local mapTable = require('scenes.' .. sceneName .. '.map')
    local allThings = mapTable["allThings"]
    local customThings = require('scenes.' .. sceneName .. '.setup')
    for _,thing in ipairs(customThings) do table.insert(allThings, thing) end

    _loadScene(mapTable["backgroundPath"], allThings, host)
end
