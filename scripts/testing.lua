x = {
    a = 1,
    b = 2,
    c = {
        y = 100
    }
}
t = x["c"]
t["y"] = t["y"] + 2
t["m"] = -2
print(x["c"]["y"])
print(x["c"]["m"])

v = x["b"]
v = v + 1000
print(x["b"])
print(v)

y = x
y["b"] = y["b"] + 10000
print(x["b"])