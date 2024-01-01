function loadScene(host, sceneName)
    local setup = require('scenes.' .. sceneName .. '.setup')
    local eventModule = require("scripts.event")
    local mapTable = require('scenes.' .. sceneName .. '.newmap')

    local populateDefinitions
    beginEvent, resumeEvent, populateDefinitions = table.unpack(eventModule)
    local customThings, eventDefinitions = table.unpack(setup)

    local spawnThings = {}
    for _,thing in ipairs(mapTable["allThings"]) do
        local spawn = {}
        for k,v in pairs(thing) do spawn[k] = v end
        for k,v in pairs(customThings[thing["name"]]) do if k ~= "events" then spawn[k] = v end end
        table.insert(spawnThings, spawn)
    end
    populateDefinitions(customThings, eventDefinitions)

    _loadScene(mapTable["backgroundPath"], spawnThings, host)
end

function spawn(host, sceneName, args)
    local setup = require('scenes.' .. sceneName .. '.setup')
    local customThings, _ = table.unpack(setup)
    local spawn = {}
    for k,v in pairs(customThings[args["baseName"]]) do if k ~= "events" then spawn[k] = v end end
    for k,v in pairs(args) do spawn[k] = v end
    return _createThing(spawn, host)
end