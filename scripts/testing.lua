-- demonstrates assignment by reference in lua. keeping it around for now
-- x = {
--     a = 1,
--     b = 2,
--     c = {
--         y = 100
--     }
-- }
-- t = x["c"]
-- t["y"] = t["y"] + 2
-- t["m"] = -2
-- print(x["c"]["y"])
-- print(x["c"]["m"])

-- v = x["b"]
-- v = v + 1000
-- print(x["b"])
-- print(v)

-- y = x
-- y["b"] = y["b"] + 10000
-- print(x["b"])

-- local w, y
-- w = "poop"
-- y = "pee"
-- z = "earwax"
-- print(w .. " " .. y .. " " .. z)
-- print(_G["w"])
-- print(_G["z"])

-- local b = {
--     c = {
--         f = 5
--     },
--     doStuff = function (self, stuff)
--         self.d = self.d + 1;
--         print(self.d)
--         print(stuff) 
--     end,
-- }
-- local a = {}

-- setmetatable (b, { __index = function (t,k) t[k] = 0; return t[k] end })
-- setmetatable(a, { __index = b })
-- setmetatable(b.c, { __index = function(t,k) print("YOOOO"); return {} end })
-- a:doStuff("whatever")
-- a:doStuff("ok")
-- a:doStuff("shut up")
-- b.c.Z["something"] = 100
-- print(b.c.Z["something"])

-- if false then
--     print("NO")
-- elseif false then
--     print("NOOOO")
-- else
--     print("YESSS")
-- end

-- function a (arg1)
--     print(arg1)
--     coroutine.yield()
--     print(arg1)
-- end
-- b = coroutine.create(a, 5)
-- coroutine.resume(b, 5)
-- coroutine.resume(b)

-- local a = {}
-- setmetatable(a, {
--         __index = function(k,t) do
--             if k == "something" then
--                 print("YO")
--                 -- GameState.itemEventId = GameState.itemEventId + 1;
--             end
--             return a[k]
--         end
--     }
-- )

-- local c = function(self) print(self.b) end

-- local a = {
--     b = 3,
-- }
-- a.c = c

-- a:c()


local a = { c = nil}
local b = a
local c = a.c
a.c = 5
print(c)