-- Manipulate things by name here for starting state that can't be stored in data.
-- Note that without some other solution, or way of writing this or other files,
-- the scene will start up exactly the same every time it is loaded.

local function setup(host)
    _createThing(
        {
            "moveAnimate", -- this should be a table or a list of table with "componentName" keys so that components can have attributes
            "autoMove",
            "standardCollider" -- maybe this gets relegated to as an option in the table for moveAnimate or something, I dunno
        },
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
            obstructionData = {}
        },
        host
    )
end

return setup