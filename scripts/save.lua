-- modified from:
-- https://stackoverflow.com/questions/9168058/how-to-dump-a-table-to-console
local function getDumpString(o)
   if type(o) == 'table' then
      local s = '{ '
      for k,v in pairs(o) do
         if type(k) ~= 'number' then k = ' ' .. k .. ' = ' else k = "" end
         if type(v) == 'string' then v = '"' .. v .. '"' end
         s = s .. k .. getDumpString(v) .. ','
      end
      return s .. '} '
   else
      return tostring(o)
   end
end

function saveMap(t, scene)
   local dumpString =  getDumpString(t)
   local dateString = os.date("%d-%m-%y__%H_%M_%S")
   print("Map Data Computed")
   io.output("scenes/" .. scene .. "/map-" .. dateString .. ".lua")
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
