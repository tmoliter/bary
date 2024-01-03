require("scripts.copy")
function loadGame(saveFile)
    print("LOAD GAME")
    local gameState = require('state.gameState')
    local saveData = require("saves." .. saveFile)
    gameState["spawn"] = deepcopy(saveData["spawn"])
    gameState["scenes"] = deepcopy(saveData["scenes"])
    print("HEY #1")
    print(gameState["spawn"])
    return saveData["spawn"] -- should return all of spawn here
end

function loadScene(host, sceneName, isEditing)
    local setup = require('scenes.' .. sceneName .. '.setup')
    local gameState = require('state.gameState')
    print("HEY #2")
    print(gameState["spawn"])
    local eventModule = require("scripts.event")
    local populateDefinitions
    beginEvent, resumeEvent, populateDefinitions = table.unpack(eventModule)
    local thingDefs, eventDefinitions = table.unpack(setup)

    local mapTable
    local playerSpawn
    if isEditing == true then
        mapTable = require('scenes.' .. sceneName .. '.newmap')
    else
        mapTable = gameState["scenes"][sceneName]
        playerSpawn = thingDefs[gameState["spawn"]["name"]]
        for k,v in pairs(gameState["spawn"]) do playerSpawn[k] = v end
    end

    local spawnThings = {}
    for _,thing in ipairs(mapTable["things"]) do -- We should read from something less permanent than the original maptable?
        local spawn = {}
        for k,v in pairs(thingDefs[thing["name"]]) do spawn[k] = v end
        for k,v in pairs(thing) do spawn[k] = v end
        table.insert(spawnThings, spawn)
    end
    if playerSpawn ~= nil then
        table.insert(spawnThings, playerSpawn)
    end
    populateDefinitions(thingDefs, eventDefinitions)

    _loadScene(mapTable["backgroundPath"], spawnThings, host)
end


function spawn(host, sceneName, args)
    local setup = require('scenes.' .. sceneName .. '.setup')
    local thingDefs, _ = table.unpack(setup)
    local spawn = {}
    for k,v in pairs(thingDefs[args["baseName"]]) do if k ~= "events" then spawn[k] = v end end
    for k,v in pairs(args) do spawn[k] = v end
    return _createThing(spawn, host)
end