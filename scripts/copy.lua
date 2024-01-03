function deepcopy(obj)
    if type(obj) ~= 'table' then return obj end
    local res = {}
    for k, v in pairs(obj) do res[deepcopy(k)] = deepcopy(v) end
    return res
end

function deepmerge(target, source)
    -- TODO: deep copy all key-values from source onto target
end
