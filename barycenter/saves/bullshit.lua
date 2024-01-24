return {
    spawn = {
        name = "zinnia",
        scene = "burg",
        x = 700,
        y = 530,
        scale = 2
    },
    scenes = {
        burg = { 
            backgroundPath = "backgrounds/Burg", 
            things = { 
                {
                    name = "otherZinnia",
                    x = 1300,
                    y = 500
                },
                {
                    name = "sailorShack",
                    x = 621,
                    y = 510,
                }
            } 
        }
    },
    inventories = {
        zinnia = {oolong = 33, mungBeanJuice = 18, tpRoll = 10},
        jordan = {}
    },
    party = {
        "jordan",
        "zinnia"
    }
}