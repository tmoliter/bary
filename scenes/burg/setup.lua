-- BEHAVIORS

local function zinniaAutoMove(hostThing, args, eventName)
    originX, originY = table.unpack {args["originX"], args["originY"]}
    while true do
        _newTask(
            {
                {
                    type = "move",
                    destinationX = originX - 150,
                    destinationY = originY - 150
                },
            }, eventName, hostThing
        )
        coroutine.yield()
        _newTask(
            {
                {
                    type = "move",
                    destinationX = originX + 35,
                    destinationY = originY - 150
                },
            }, eventName, hostThing
        )
        coroutine.yield()
        _newTask(
            {
                {
                    type = "move",
                    destinationX = originX,
                    destinationY = originY
                },
            }, eventName, hostThing
        )
        coroutine.yield()
    end
end

-- EVENTS

local function zinniaTalkA(hostThing, args, eventName)
    _newTask(
        {
            {
                type = "phrase",
                text = "Didn't I tell you not to come around here",
                x = 150,
                y = 150,
                width = 400,
                height = 100,
                scrollType = "continuous",
                gridLimitsX = 1000,
                gridLimitsY = 1000,
                frames = 250,
                blocking = true
            }
        }, eventName, hostThing
    )
end

local function zinniaTalkB(hostThing, args, eventName)
    _newTask(
        {
            {
                type = "move",
                offsetX = -100,
                offsetY = 50
            },
            {
                type = "phrase",
                text = "doodoo",
                x = 30,
                y = 40, 
                width = 80,
                height = 50,
                scrollType = "continuous",
                gridLimitsX = 100,
                gridLimitsY = 100,
                frames = 125,
                blocking = true
            }
        }, eventName, hostThing
    )
    coroutine.yield()
    _newTask(
        {
            {
                type = "move",
                thingName = "otherZinnia",
                offsetX = -300,
                offsetY = 300
            },
            {
                type = "phrase",
                text = "poopoo",
                x = 300,
                y = 100, 
                width = 100,
                height = 50,
                scrollType = "continuous",
                gridLimitsX = 1000,
                gridLimitsY = 1000,
                frames = 125,
                blocking = true
            }
        }, eventName, hostThing
    )
end

local eventDefinitions = {
    followZinnia = {
        interact =  {
            type = "custom",
            customCoroutine = zinniaTalkA
        },
        interact_1 = {
            type = "custom",
            customCoroutine = zinniaTalkB
        }
    }
}

local customThings = {
    {
        name = "otherZinnia",
        spriteDataVector = {
            {
                xOffset = 0,
                height = 0,
                layer = 0,
                textureName = "zinnia",
                renderOffset = 0,
                width = 0,
                yOffset = 0,
                sourceX = 0,
                sourceY = 0
            }
        },
        x = 1000,
        y = 500,
        obstructionData = {},
        components = {
            {
                type = "moveAnimate",
                standardCollider = true
            },
            {
                type = "autoMove",
                eventName = "autoMove",
            }
        },
        events = {
            -- autoMove = {
            --     type = "randomAutoMove",
            --     variance = 150
            -- },
            autoMove = {
                type = "custom",
                customCoroutine = zinniaAutoMove
            },
            interact =  { -- This has the same effect as zinniaTalk, but is stored as data
                type = "simpleMessages",
                phrases = {
                    {
                        text = "Hey what's happening bro",
                        x = 300,
                        y = 150,
                        width = 400,
                        height = 100,
                        scrollType = "continuous",
                        gridLimitsX = 1000,
                        gridLimitsY = 1000,
                    },
                    {
                        text = "Didn't I tell you not to come around here",
                        x = 150,
                        y = 150,
                        width = 400,
                        height = 100,
                        scrollType = "continuous",
                        gridLimitsX = 1000,
                        gridLimitsY = 1000,
                    },
                }
            },
            interact_1 = {
                type = "custom",
                customCoroutine = zinniaTalkB
            },
        }
    }
}

-- Manipulate things by name here for starting state that can't be stored in data.
-- Note that without some other solution, or way of writing this or other files,
-- the scene will start up exactly the same every time it is loaded.

-- local function setup(host) -- not currently in use
    -- _createThing(
    --     {
    --         name = "otherZinnia",
    --         spriteDataVector = {
    --             {
    --                 xOffset = 0,
    --                 height = 0,
    --                 layer = 0,
    --                 textureName = "zinnia",
    --                 renderOffset = 0,
    --                 width = 0,
    --                 yOffset = 0,
    --                 sourceX = 0,
    --                 sourceY = 0
    --             }
    --         },
    --         x = 1000,
    --         y = 500,
    --         obstructionData = {},
    --         components = {
    --             { 
    --                 type = "moveAnimate",
    --                 standardCollider = true
    --             },
    --             { type = "autoMove" }
    --         },
    --     },
    --     host
    -- )
-- end

return { customThings, eventDefinitions }