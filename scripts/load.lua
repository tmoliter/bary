require("scripts.copy")
require("config.settings")
gameState = require('state.gameState')
standardEvents = require('scripts.standardEvents')
itemDefinitions = require(GAME_PATH .. ".definitions.itemDefinitions")
sceneManager = nil
local eventModule = require("scripts.event")
beginEvent = eventModule.beginEvent
resumeEvent = eventModule.resumeEvent
local baseResources = require('base.resources')

function loadBaseResources()
    local Resources = require('scripts.resourceobject')
    local baseResources = Resources.new({ baseTextures = baseResources.UI })
    return baseResources:getTextures()
end

function loadGame(saveFile)
    local saveData = require(GAME_PATH .. ".saves." .. saveFile)

    gameState:fresh()
    gameState.spawn = deepcopy(saveData.spawn)
    gameState.scenes = deepcopy(saveData.scenes)
    for inventoryName,items in pairs(saveData.inventories) do
        gameState:addInventory(inventoryName, items)
    end
    for _,partyMemberName in ipairs(saveData.party) do
        table.insert(gameState.party, partyMemberName)
    end
    return saveData.spawn
end

function loadScene(host, sceneName, isEditing, newSceneManager)
    sceneManager = newSceneManager
    local setup = require(GAME_PATH .. '.scenes.' .. sceneName .. '.setup')
    local resources, thingDefs, sceneEvents = table.unpack(setup)

    local mapTable
    local playerSpawn

    if isEditing == true then
        resources.baseTextures = getMerge({resources.baseTextures or {}, baseResources.editorTextures})
        mapTable = require(GAME_PATH .. '.scenes.' .. sceneName .. '.map')
    else
        mapTable = gameState["scenes"][sceneName]
        playerSpawn = thingDefs[gameState["spawn"]["name"]]
        for k,v in pairs(gameState["spawn"]) do playerSpawn[k] = v end
    end

    eventModule.populate(thingDefs, sceneEvents)

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

    _loadScene(resources.background, spawnThings, { textures = resources:getTextures() }, host)
end


function spawn(host, sceneName, args)
    local setup = require(GAME_PATH .. '.scenes.' .. sceneName .. '.setup')
    local _, thingDefs, _ = table.unpack(setup)
    local spawn = {}
    for k,v in pairs(thingDefs[args["baseName"]]) do if k ~= "events" then spawn[k] = v end end
    for k,v in pairs(args) do spawn[k] = v end
    return _createThing(spawn, host)
end