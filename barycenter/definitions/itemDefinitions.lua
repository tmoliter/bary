local itemDefinitions = {
    mungBeanJuice = {
        name = "Mung Bean Juice",
        plural = "Mung Bean Juices"
    },
    oolong = {
        name = "Oolong Tea",
        plural = "Oolong Teas"
    }
}

itemDefinitions.mungBeanJuice.use = function(hostThing, args)
    print("USING " .. args.amount .. " MUNG BEAN JUICES ON " .. args.target)
    gameState.inventories[args.source]:add("oolong", args.amount * 2)
    local phrase = {
        type = "phrase",
        text = args.source .. " GOT 2 OOLONGS",
        x = 300,
        y = 150,
        width = 500,
        height = 30,
        scrollType = "continuous",
        gridLimitsX = 1000,
        gridLimitsY = 1000,
    }
    _newTask(
        { 
            {
                type = "pauseMoves",
                all = true,
            },
            phrase
        }, args.eventName, hostThing
    )
    coroutine.yield()
    phrase.text = args.source .. " NOW HAS " .. gameState.inventories[args.source]:count("mungBeanJuice") .. " MUNG BEAN JUICES"
    phrase.x = 400
    phrase.y = 400
    _newTask({ phrase }, args.eventName, hostThing)
    coroutine.yield()
    _newTask({{
        type = "pauseMoves",
        all = true,
        unpause = true
    }}, args.eventName, hostThing
    )
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