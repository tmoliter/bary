local itemDefinitions = {
    mungBeanJuice = {
        use = function(hostThing, args, eventName)
            print("USING " .. args.amount .. " MUNG BEAN JUICES ON " .. args.target)
            gameState.inventories[args.target]:add("oolong", args.amount * 2)
            local text = args.target .. " NOW HAS " .. gameState.inventories[args.target]:count("oolong") .. " OOLONGS"
            print(text)
            local phrase = {
                type = "phrase",
                text = text,
                x = 300,
                y = 150,
                width = 300,
                height = 30,
                scrollType = "continuous",
                gridLimitsX = 1000,
                gridLimitsY = 1000,
                blocking = true
            }
            _newTask(
                { phrase }, eventName, hostThing
            )
            coroutine.yield()
            phrase.text = args.source .. " NOW HAS " .. gameState.inventories[args.source]:count("mungBeanJuice") .. " MUNG BEAN JUICES"
            phrase.x = 400
            phrase.y = 400
            _newTask({ phrase }, eventName, hostThing)
        end
    },
    oolong = {
        use = function(args)
            print("used " .. args.amount .. " oolong")
        end
    }
}

setmetatable(itemDefinitions, {
    __index = function(t,k)
        print("No item definition for " .. k .. "found!")
        error()
    end
})

return itemDefinitions