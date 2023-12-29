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
                variance = 50,
                standardBehavior = "randomAutoMove",
                pauseTime = 100
            }
        },
    }
}

-- Manipulate things by name here for starting state that can't be stored in data.
-- Note that without some other solution, or way of writing this or other files,
-- the scene will start up exactly the same every time it is loaded.

local function setup(host) -- not currently in use
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
end

return customThings