-- modified from:
-- https://stackoverflow.com/questions/9168058/how-to-dump-a-table-to-console
local function getDumpString(o)
   if type(o) ~= 'table' then
      return tostring(o)
   end
   local s = '{ '
   for k, v in pairs(o) do
      local keyStr = type(k) == 'number' and "" or (' ' .. k .. ' = ')
      local valStr = type(v) == 'string' and ('"' .. v .. '"') or getDumpString(v)
      s = s .. keyStr .. valStr .. ','
   end
   return s .. '} '
end

local function buildSaveData(spawn)
    local inventories = {}
    for name, inventory in pairs(gameState.inventories) do
        inventories[name] = inventory.items
    end
    return {
        spawn = spawn,
        scenes = gameState.scenes,
        quests = gameState.quests,
        inventories = inventories,
        party = gameState.party,
    }
end

function saveGame(saveName, spawn)
    local dumpString = getDumpString(buildSaveData(spawn))
    io.output("games/" .. settings.GAME_NAME .. "/saves/" .. saveName .. ".lua")
    io.write("return " .. dumpString)
    io.flush()
    print("SAVED game to " .. saveName)
end

function saveMap(t, scene)
   local dumpString =  getDumpString(t)
   local dateString = os.date("%d-%m-%y__%H_%M_%S")
   print("Map Data Computed")
   io.output("games/" .. settings.GAME_NAME .. "/scenes/" .. scene .. "/map-" .. dateString .. ".lua")
   io.write("return " .. dumpString)
   io.flush()
end

function printMap(things, sceneName)
   local dumpString = getDumpString(things)
   local dateString = os.date("%d-%m-%y__%H_%M_%S")
   local output = "things = " .. dumpString
   print("Map Data Computed")
   print(output)
   io.output("working_files/thingData/" .. sceneName .. "/THINGDUMP-" .. dateString .. ".lua")
   io.write(output)
   io.flush()
end

function printThing(thing, sceneName)
   thing["name"] = string.gsub(thing["name"], "%s+", "")

   local dumpString = getDumpString(thing)
   local dateString = os.date("%d-%m-%y__%H_%M_%S")
   local output = thing["name"] .. " = " .. dumpString
   print(output)
   io.output("working_files/thingData/" .. sceneName .. "/" .. thing["name"] .. "-" .. dateString .. ".lua")
   io.write(output)
   io.flush()
end
