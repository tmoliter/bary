return {
    spawn = {
        name = "zinnia",
        scene = "burg",
        x = 700,
        y = 530,
        scale = 2
    },
    quests = {
        some = { damn = { quest = "completed" } }
    },
    scenes = {
        burg = {
            things = {
                burgChest = { opened = true, activeSprites = { 0 } }
            }
        }
    },
    inventories = {
        zinnia = { oolong = 33, mungBeanJuice = 18, tpRoll = 10 },
        jordan = {}
    },
    party = {
        "zinnia",
        "jordan"
    }
}
