local itemDefinitions = {
    mungBeanJuice = {
        name = "Mung Bean Juice"
    },
    oolong = {
        name = "Oolong Tea"
    }
}

itemDefinitions.mungBeanJuice.use = function(hostThing, args)
    print("USING " .. args.amount .. " MUNG BEAN JUICES ON " .. args.target)
    gameState.inventories[args.target]:add("oolong", args.amount * 2)
    local phrase = {
        type = "phrase",
        text = args.target .. " NOW HAS " .. gameState.inventories[args.target]:count("oolong") .. " OOLONGS",
        x = 300,
        y = 150,
        width = 500,
        height = 30,
        scrollType = "continuous",
        gridLimitsX = 1000,
        gridLimitsY = 1000,
        blocking = true
    }
    _newTask(
        { phrase }, args.eventName, hostThing
    )
    coroutine.yield()
    phrase.text = args.source .. " NOW HAS " .. gameState.inventories[args.source]:count("mungBeanJuice") .. " MUNG BEAN JUICES"
    phrase.x = 400
    phrase.y = 400
    _newTask({ phrase }, args.eventName, hostThing)
    coroutine.yield()
    local options = {}
    local i = 1
    for k,v in pairs(itemDefinitions) do options[i] = { selectionText = v.name, value = k }; i = i+1; end
    _newTask({
        {
            type = "menu",
            options = options,
            x = 300,
            y = 150,
            width = 340,
            height = 60,
            maxColumns = 2,
            closeOnDestroy = true,
            blocking = true
        }
    }, args.eventName, hostThing)
    _, args = coroutine.yield()
    phrase.text = "You chose '" .. itemDefinitions[args.selection].name .. "' (" .. args.selection .. ")"
    _newTask({ phrase }, args.eventName, hostThing)
end

itemDefinitions.oolong.use = function(args)
    print("used " .. args.amount .. " oolong")
end


setmetatable(itemDefinitions, {
    __index = function(t,k)
        print("No item definition for " .. k .. " found!")
        error()
    end
})

return itemDefinitions