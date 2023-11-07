thing = {
    name = "Sailor Shack",
    x = 300,
    y = 300,
    spriteDataVector = {
        {
            layer = 0,
            renderOffset = -33,
            xOffset = 0,
            yOffset = 0,
            sourceX = 0,
            sourceY = 50,
            width = 50,
            height = 50,
            textureName = "sailorshack"
        },
        {
            layer = 0,
            renderOffset = 0,
            xOffset = 50,
            yOffset = 50,
            sourceX = 99,
            sourceY = 175,
            width = 50,
            height = 50,
            textureName = "sailorshack"
        }
    },
    obstructionData = {
        {
            layer = 0,
            rays = {
                {
                    aX = 0,
                    aY = 0,
                    bX = 50,
                    bY = 0,
                },
                {
                    aX = 50,
                    aY = 0,
                    bX = 50,
                    bY = 50,
                },
                {
                    aX = 50,
                    aY = 50,
                    bX = 0,
                    bY = 50
                },
                {
                    aX = 0,
                    aY = 50,
                    bX = 0,
                    bY = 0
                }
            }
        },
        {
            layer = 1,
            rays = {
                {
                    aX = 50,
                    aY = 50,
                    bX = 100,
                    bY = 50,
                },
                {
                    aX = 100,
                    aY = 50,
                    bX = 100,
                    bY = 100,
                },
                {
                    aX = 100,
                    aY = 100,
                    bX = 50,
                    bY = 100
                },
                {
                    aX = 50,
                    aY = 100,
                    bX = 50,
                    bY = 50
                }
            }
        }
    }
}
