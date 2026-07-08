require("scripts.copy")
require("config.settings")
gameState = require('state.gameState')
standardEvents = require('scripts.standardEvents')
itemDefinitions = require(GAME_PATH .. ".definitions.itemDefinitions")
local eventModule = require("scripts.event")
local bundles = require("scripts.bundles")
beginEvent = eventModule.beginEvent
resumeEvent = eventModule.resumeEvent
fireCollidable = eventModule.fireCollidable
clearAllEvents = eventModule.clearAllEvents
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
    gameState.quests = deepcopy(saveData.quests or {})
    for inventoryName,items in pairs(saveData.inventories) do
        gameState:addInventory(inventoryName, items)
    end
    for _,partyMemberName in ipairs(saveData.party) do
        table.insert(gameState.party, partyMemberName)
    end
    return saveData.spawn
end

function newGame(sceneName)
    local map = require(GAME_PATH .. '.scenes.' .. sceneName .. '.map')
    if map.spawn == nil then
        error("newGame: scene '" .. sceneName .. "' has no `spawn` in its map.lua")
    end

    gameState:fresh()
    local spawn = deepcopy(map.spawn)
    spawn.scene = sceneName
    gameState.spawn = spawn
    gameState.party = { spawn.name }
    gameState:addInventory(spawn.name, {})
    return spawn
end

function loadScene(host, sceneName, isEditing)
    local setup = require(GAME_PATH .. '.scenes.' .. sceneName .. '.setup')
    local resources, thingDefs, sceneEvents = table.unpack(setup)

    local baseMap = require(GAME_PATH .. '.scenes.' .. sceneName .. '.map')
    local things = baseMap["things"]
    local playerSpawn

    if isEditing == true then
        resources.baseTextures = getMerge({resources.baseTextures or {}, baseResources.editorTextures})
    else
        things = gameState:resolveSceneThings(sceneName, baseMap["things"])
        gameState.currentScene = sceneName
        if gameState["spawn"] then
            playerSpawn = thingDefs[gameState["spawn"]["name"]]
            for k,v in pairs(gameState["spawn"]) do playerSpawn[k] = v end
        end
    end

    bundles.expandOpenableDefs(thingDefs)

    eventModule.populate(thingDefs, sceneEvents)

    local spawnThings = {}
    for _,savedThing in ipairs(things) do
        local spawn = {}
        local thingDef = thingDefs[savedThing["name"]]
        for k,v in pairs(thingDef) do spawn[k] = v end
        for k,v in pairs(savedThing) do spawn[k] = v end
        bundles.applyOpenable(spawn)
        table.insert(spawnThings, spawn)
    end
    if playerSpawn ~= nil then
        table.insert(spawnThings, playerSpawn)
    end
    gameState["spawn"] = nil

    _loadScene(resources.background, spawnThings, { textures = resources:getTextures() }, host)
end


function spawn(host, sceneName, args)
    local setup = require(GAME_PATH .. '.scenes.' .. sceneName .. '.setup')
    local _, thingDefs, _ = table.unpack(setup)
    local spawn = {}
    for k,v in pairs(thingDefs[args["baseName"]]) do if k ~= "events" then spawn[k] = v end end
    for k,v in pairs(args) do spawn[k] = v end
    bundles.applyOpenable(spawn)
    return _createThing(spawn, host)
end
