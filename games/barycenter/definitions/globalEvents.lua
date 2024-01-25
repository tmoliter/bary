function inventoryMenu(hostThing, args)
    local function nameAndAmount (itemDef, amount)
        if amount == 1 then
            return amount .. " " .. itemDef.name
        end
        return  amount .. " " .. itemDef.plural
    end
    local function getInventoryOptions()
        local options = {}
        local i = 1
        for itemKey, amount in pairs(gameState.inventories[args.inventoryName].items) do
            options[i] = { 
                selectionText = nameAndAmount(itemDefinitions[itemKey], amount), 
                value = itemKey
            }
            i = i+1
        end
        return options
    end
    local actionOptions = {
        {
            selectionText = "use",
            value = "use"
        },
        {
            selectionText = "drop",
            value = "drop"
        }
    }


    _newTask({
        {
            type = "pauseMoves",
            all = true,
        },
        {
            type = "menu",
            options = getInventoryOptions(),
            x = 300,
            y = 150,
            width = 340,
            height = 60,
            maxColumns = 2,
            blocking = true
        }
    }, args.eventName, hostThing)

    local _, invMenuArgs = coroutine.yield()

    while invMenuArgs.selection ~= "" do
        _newTask({
            {
                type = "menu",
                options = actionOptions,
                x = 350,
                y = 250,
                width = 80,
                height = 60,
                maxColumns = 1,
                blocking = true
            }
        }, args.eventName, hostThing)

        local _, actionMenuArgs = coroutine.yield()

        local action = actionMenuArgs.selection
        local target = nil
        if action == "use" then
            local sufficient, remaining = gameState.inventories[args.inventoryName]:use(invMenuArgs.selection, 1)
            if sufficient ~= true then
                print("INSUFFICIENT")
                return
            end

            if itemDefinitions[invMenuArgs.selection].needsTarget then
                local targetOptions = {}
                for _,partyMemberName in ipairs(gameState.party) do 
                    table.insert(targetOptions, {
                    selectionText = partyMemberName,
                    value = partyMemberName
                    })
                end
                _newTask({{
                    type = "menu",
                    menu = actionMenuArgs.menu,
                    options = targetOptions
                }}, args.eventName, hostThing)

                local _, actionMenuArgs = coroutine.yield()
                
                if actionMenuArgs.selection ~= "" then
                    target = actionMenuArgs.selection
                else
                    _newTask({{
                        type = "menu",
                        menu = actionMenuArgs.menu,
                        close = true
                    }}, args.eventName, hostThing)
                    goto continue
                end
            end

            local args = {
                eventDefinition = {
                    type = "custom", 
                    customCoroutine = itemDefinitions[invMenuArgs.selection].use,
                    amount = 1,
                    target = target,
                    source = args.inventoryName
                },
                eventName = "item_" .. gameState.itemEventId,
                catalyst = "item"
            }
            _newTask({
                {
                    type = "fireEvent",
                    args = args
                },
                {
                    type = "menu",
                    menu = actionMenuArgs.menu,
                    close = true
                },
            }, args.eventName, hostThing)
            break
        elseif action == "drop" then
            gameState.inventories[args.inventoryName]:drop(invMenuArgs.selection, 1)
        end
        _newTask({
            {
                type = "menu",
                menu = actionMenuArgs.menu,
                close = true
            },
            {
                type = "menu",
                menu = invMenuArgs.menu,
                options = getInventoryOptions()
            }
        }, args.eventName, hostThing)

        _, invMenuArgs = coroutine.yield()
        ::continue::
    end
    _newTask({
        {
            type = "menu",
            menu = invMenuArgs.menu,
            close = true,
        },
        {
            type = "pauseMoves",
            all = true,
            unpause = true
        }
    }, args.eventName, hostThing)
end

return {
    inventoryMenu = inventoryMenu
}
