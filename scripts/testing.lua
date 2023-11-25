x = {}
if x["y"] == nil then
    print("HEY")
    x["y"] = {}
end
x["y"]["z"] = 5
print(x)
print(x["y"])
print(x["y"]["z"])
print(x["y"]["A"])
if x["y"] == nil then
    print("HEY")
    x["y"] = {}
end
x["y"]["A"] = 10
poop = x["y"]["A"] 
poop = poop + 1
print(x["y"]["A"])
