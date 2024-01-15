require("scripts.copy")
require("config.settings")
gameState = require('state.gameState')
standardEvents = require('scripts.standardEvents')
itemDefinitions = require(settings.GAME_NAME .. ".definitions.itemDefinitions")
sceneManager = nil
local eventModule = require("scripts.event")
beginEvent = eventModule.beginEvent
resumeEvent = eventModule.resumeEvent

function loadGame(saveFile)
    local saveData = require(settings.GAME_NAME .. ".saves." .. saveFile)

    gameState:fresh()
    gameState.spawn = deepcopy(saveData.spawn)
    gameState.scenes = deepcopy(saveData.scenes)
    for inventoryName,items in pairs(saveData.inventories) do
        gameState:addInventory(inventoryName, items)
    end
    return saveData.spawn
end

function loadScene(host, sceneName, isEditing, newSceneManager)
    sceneManager = newSceneManager
    local setup = require(settings.GAME_NAME .. '.scenes.' .. sceneName .. '.setup')
    local thingDefs = table.unpack(setup)

    local mapTable
    local playerSpawn

    if isEditing == true then
        -- TESTING
        gameState:addInventory("zinnia", {oolong = 100, mungBeanJuice = 3})
        gameState:addInventory("jordan")
        -- END TESTING
        mapTable = require(settings.GAME_NAME .. '.scenes.' .. sceneName .. '.map')
    else
        mapTable = gameState["scenes"][sceneName]
        playerSpawn = thingDefs[gameState["spawn"]["name"]]
        for k,v in pairs(gameState["spawn"]) do playerSpawn[k] = v end
    end

    eventModule.populate(thingDefs)

    local spawnThings = {}
    for _,savedThing in ipairs(mapTable["things"]) do
        local spawn = {}
        local thingDef = thingDefs[savedThing["name"]]
        for k,v in pairs(thingDef) do spawn[k] = v end -- deepMerge
        for k,v in pairs(savedThing) do spawn[k] = v end -- deepMerge
        table.insert(spawnThings, spawn)
    end
    if playerSpawn ~= nil then
        table.insert(spawnThings, playerSpawn)
    end

    _loadScene(mapTable["backgroundPath"], spawnThings, host)
end


function spawn(host, sceneName, args)
    local setup = require(settings.GAME_NAME .. '.scenes.' .. sceneName .. '.setup')
    local thingDefs, _ = table.unpack(setup)
    local spawn = {}
    for k,v in pairs(thingDefs[args["baseName"]]) do if k ~= "events" then spawn[k] = v end end
    for k,v in pairs(args) do spawn[k] = v end
    return _createThing(spawn, host)
end