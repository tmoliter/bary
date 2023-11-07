-- modified from:
-- https://stackoverflow.com/questions/9168058/how-to-dump-a-table-to-console
function dump(t)
   function getDumpString(o)
      if type(o) == 'table' then
         local s = '{ '
         for k,v in pairs(o) do
            if type(k) ~= 'number' then k = '"'..k..'"' end
            s = s .. '['..k..'] = ' .. getDumpString(v) .. ','
         end
         return s .. '} '
      else
         return tostring(o)
      end
   end
   io.output("test.lua")
   io.write("allThings = " .. getDumpString(t))
end