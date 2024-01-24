function deepcopy(obj)
    if type(obj) ~= 'table' then return obj end
    local res = {}
    for k, v in pairs(obj) do res[deepcopy(k)] = deepcopy(v) end
    return res
end

function deepmerge(target, source)
    for k,v in pairs(source) do
        if type(target[k]) == 'table' and type(source[k]) == 'table' then
            deepmerge(target[k], source[k])
        else
            target[k] = v 
        end
    end
end

function printtable(t, n)
    n = n or 0
    local indent = ""
    for i=1,n do indent = indent .. "    " end
    for k,v in pairs(t) do
        if type(v) == 'table' then
            print(indent .. k .. " =")
            printtable(v, n+1)
        else
            print(indent .. k .. " = " .. tostring(v))
        end
    end
end
