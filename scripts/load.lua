function loadScene(host, sceneName)
    local eventModule = require("scripts.event")
    local mapTable = require('scenes.' .. sceneName .. '.newmap')
    local setup = require('scenes.' .. sceneName .. '.setup')

    local populateDefinitions
    beginEvent, resumeEvent, populateDefinitions = table.unpack(eventModule)
    local customThings, eventDefinitions = table.unpack(setup)

    local spawnThings = {}
    for _,thing in ipairs(mapTable["allThings"]) do
        local spawn = {}
        for k,v in pairs(thing) do if k ~= "events" then spawn[k] = v end end
        for k,v in pairs(customThings[thing["name"]]) do spawn[k] = v end
        table.insert(spawnThings, spawn)
    end
    populateDefinitions(customThings, eventDefinitions)

    _loadScene(mapTable["backgroundPath"], spawnThings, host)
end
