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

function dump(t, scene)
   print("Map Data Computed")
   io.output("scenes/" .. scene .. "/map.lua")
   io.write("return " .. getDumpString(t))
   io.flush()
end

function printThing(thing, sceneName)
   thing["name"] = string.gsub(thing["name"], "%s+", "")
   local output = thing["name"] .. " = " .. getDumpString(thing)
   print(output)
   io.output("scenes/" .. sceneName .. "/tempThings/" .. thing["name"] .. ".lua")
   io.write(output)
   io.flush()
end
