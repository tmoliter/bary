-- modified from:
-- https://stackoverflow.com/questions/9168058/how-to-dump-a-table-to-console
function dump(t)
   function getDumpString(o)
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
   print("Map Data Computed")
   io.output("storage/maps/burg.lua")
   io.write("allThings = " .. getDumpString(t))
end