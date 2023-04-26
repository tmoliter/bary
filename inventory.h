#ifndef INVENTORY_H
#define INVENTORY_H
#include <map>
#include <string>
#include <vector>
#include <SDL2/SDL_mixer.h>

using namespace std;

enum class Attribute {
    Might,
    Wonder,
    Logic,
    Chill,
    Guile
};

enum class Quality {
    fire,
    water,
    physical,
    psychological,
    funny,
    boring,
    annoying
};

enum class Status {
    poisoned,
    drunk,
    enlightened,
    guilty,
    apathetic,
    irreverent,
    comatose
};


enum class PlayerCharacter {
    Zinnia,
    Paul,
    Jordan,
    Jeff
};

enum class InventoryItem {
    // Money
    Dollar,
    // Consumable Foods
    Oolong,
    Sundae,
    Toast,
    BeanJuice,
    // Unique Quest Items
    WeirdBook,
    Trash,
};

enum class ItemType {
    money,
    consumable,
    armor,
    weapon
};

struct Item {
    Item(ItemType type, InventoryItem iN, string dN,  string d) : 
        itemType(type),
        inventoryName(iN),
        displayName(dN), 
        description(d) 
    {};
    virtual ~Item() {};
    ItemType itemType;
    InventoryItem inventoryName;
    string displayName;
    string description;
};

struct ConsumableEffect {
    ConsumableEffect(int h, int e, vector<Status> aS, vector<Status> rS, string uM) : 
        hp(h),
        ec(e),
        applyStatus(aS),
        removeStatus(rS),
        useMessage(uM)
    {};
    int hp;
    int ec;
    vector<Status> applyStatus;
    vector<Status> removeStatus;
    string useMessage;
};

struct Consumable : public Item {
    Consumable(InventoryItem iN, string dN,  string d, ConsumableEffect ce) : 
        Item(ItemType::consumable, iN, dN, d), 
        effect(ce) 
    {};
    ConsumableEffect effect;
};

struct Equipment : public Item { // Weapon/Armor
    Equipment(ItemType type, InventoryItem iN, string dN,  string d, int bF, vector<Status> s, vector<Quality> q) :
        Item(type, iN, dN, d),
        battleFactor(bF),
        status(s),
        qualities(q)
    {};
    int battleFactor; // Damage/Resistance to damage
    vector<Status> status;
    vector<Quality> qualities;
};

// struct SpecialEffectItem : public Item {
//     function pointer
// }

namespace inventory {

    // inventory management
    map<PlayerCharacter, map<InventoryItem, int>> currentItems;
    vector<pair<Item, int>> getInventory(PlayerCharacter pc);
    ConsumableEffect consumeOne(PlayerCharacter pc, InventoryItem ii);
    int addOne(PlayerCharacter pc, InventoryItem ii); // Returns count
    int removeOne(PlayerCharacter pc, InventoryItem ii); // Returns count

    // item definitions
    Item getItemData(InventoryItem itemName);
};

#endif
