require("scripts.copy")
function loadGame(saveFile)
    local gameState = require('state.gameState')
    local saveData = require("saves." .. saveFile)
    gameState["spawn"] = deepcopy(saveData["spawn"])
    gameState["scenes"] = deepcopy(saveData["scenes"])
    return saveData["spawn"] -- should return all of spawn here
end

function loadScene(host, sceneName, isEditing)
    local setup = require('scenes.' .. sceneName .. '.setup')
    local gameState = require('state.gameState')
    local eventModule = require("scripts.event")
    local populateDefinitions
    beginEvent, resumeEvent, populateDefinitions = table.unpack(eventModule)
    local thingDefs = table.unpack(setup)

    local mapTable
    local playerSpawn
    if isEditing == true then
        mapTable = require('scenes.' .. sceneName .. '.map')
    else
        mapTable = gameState["scenes"][sceneName]
        playerSpawn = thingDefs[gameState["spawn"]["name"]]
        for k,v in pairs(gameState["spawn"]) do playerSpawn[k] = v end
    end

    local spawnThings = {}
    for _,thing in pairs(thingDefs) do
        populateDefinitions(thing)
    end
    for _,thing in ipairs(mapTable["things"]) do
        local spawn = {}
        local thingDef = thingDefs[thing["name"]]
        for k,v in pairs(thingDefs[thing["name"]]) do spawn[k] = v end -- deepMerge
        for k,v in pairs(thing) do spawn[k] = v end -- deepMerge
        table.insert(spawnThings, spawn)
    end
    if playerSpawn ~= nil then
        table.insert(spawnThings, playerSpawn)
    end

    _loadScene(mapTable["backgroundPath"], spawnThings, host)

    -- TESTING
    gameState:addInventory("zinnia", {oolong = 100})
    gameState.inventories.zinnia:use("mungBeanJuice", 3, "zinnia")
    gameState.inventories.zinnia:add("mungBeanJuice", 3)
    gameState.inventories.zinnia:use("mungBeanJuice", 2, "zinnia")
    gameState.inventories.zinnia:use("mungBeanJuice", 2, "zinnia")
    gameState.inventories.zinnia:use("mungBeanJuice", 1, "zinnia")
    gameState.inventories.zinnia:use("oolong", 3, "zinnia")
    gameState.inventories.zinnia:use("oolong", 4, "zinnia")

    gameState:addInventory("jordan")
    gameState.inventories.jordan:use("mungBeanJuice", 3, "jordan")
    gameState.inventories.jordan:add("mungBeanJuice", 3)
    gameState.inventories.jordan:use("mungBeanJuice", 2, "jordan")
    gameState.inventories.jordan:use("mungBeanJuice", 2, "jordan")
    gameState.inventories.jordan:use("mungBeanJuice", 1, "jordan")
    gameState.inventories.jordan:use("oolong", 3, "jordan")
    gameState.inventories.jordan:use("oolong", 4, "jordan")
    -- END TESTING
end


function spawn(host, sceneName, args)
    local setup = require('scenes.' .. sceneName .. '.setup')
    local thingDefs, _ = table.unpack(setup)
    local spawn = {}
    for k,v in pairs(thingDefs[args["baseName"]]) do if k ~= "events" then spawn[k] = v end end
    for k,v in pairs(args) do spawn[k] = v end
    return _createThing(spawn, host)
end