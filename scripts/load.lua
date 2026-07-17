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
clearEvent = eventModule.clearEvent
local baseResources = require('base.resources')

-- gameManager event names to auto-fire once the current scene finishes loading.
-- Set per-load in loadScene, fired by fireSceneLoadEvents (from Scene::EnterLoaded).
local sceneLoadEvents = {}

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

    -- Merge global playable-character defs so scenes don't each redefine players.
    -- A scene-local def of the same name wins. deepcopy because playerSpawn mutates
    -- its def in place below (and thingDefs/registry tables are require-cached).
    resources.ownTextures = resources.ownTextures or {}
    local fieldPlayers = require(GAME_PATH .. '.definitions.fieldPlayers')
    for playerName, entry in pairs(fieldPlayers) do
        if thingDefs[playerName] == nil then
            thingDefs[playerName] = deepcopy(entry.def)
        end
        for texName, texPath in pairs(entry.textures or {}) do
            if resources.ownTextures[texName] == nil then
                resources.ownTextures[texName] = texPath
            end
        end
    end

    local baseMap = require(GAME_PATH .. '.scenes.' .. sceneName .. '.map')
    local things = baseMap["things"]
    local playerSpawn

    sceneLoadEvents = {} -- reset each load; only play mode arms them
    if isEditing == true then
        resources.baseTextures = getMerge({resources.baseTextures or {}, baseResources.editorTextures})
    else
        things = gameState:resolveSceneThings(sceneName, baseMap["things"])
        gameState.currentScene = sceneName
        sceneLoadEvents = baseMap["onLoad"] or {}
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
        spawn.baseName = savedThing.name
        if savedThing.id then spawn.name = savedThing.id end
        bundles.applyOpenable(spawn)
        table.insert(spawnThings, spawn)
    end
    if playerSpawn ~= nil then
        table.insert(spawnThings, playerSpawn)
    end
    gameState["spawn"] = nil

    _loadScene(resources.background, spawnThings, { textures = resources:getTextures() }, host)
end

-- Called from Scene::EnterLoaded once the scene is built and the player exists.
-- Fires the scene's declared onLoad events as gameManager events, hosted by the
-- player (a convenient Host; the events aren't tied to any thing's colliders).
-- Once-only events (e.g. a tutorial) self-guard via gameState quests.
function fireSceneLoadEvents(hostThing)
    for _, eventName in ipairs(sceneLoadEvents) do
        beginEvent(hostThing, { thingName = "gameManager", eventName = eventName })
    end
end


function spawn(host, sceneName, args)
    local setup = require(GAME_PATH .. '.scenes.' .. sceneName .. '.setup')
    local _, thingDefs, _ = table.unpack(setup)
    local spawn = {}
    for k,v in pairs(thingDefs[args["baseName"]]) do if k ~= "events" then spawn[k] = v end end
    for k,v in pairs(args) do spawn[k] = v end
    spawn.baseName = args["baseName"]
    if args.id then spawn.name = args.id end
    bundles.applyOpenable(spawn)
    return _createThing(spawn, host)
end
