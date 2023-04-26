#include "inventory.h"

bool _compareItems (Item a, Item b) {
    if( static_cast<int>(b.itemType) < static_cast<int>(a.itemType))
        return true;
}

vector<pair<Item, int>> inventory::getInventory(PlayerCharacter pc) {
    vector<pair<Item, int>> itemList;
    for (auto const& [itemType, count] : currentItems[pc])
        itemList.push_back({getItemData(itemType), count});
    sort(itemList.begin(), itemList.end(), _compareItems);
    return itemList;
}

ConsumableEffect inventory::consumeOne(PlayerCharacter pc, InventoryItem ii) {
    Item* item = &getItemData(ii);
    Consumable* consumable = dynamic_cast<Consumable*>(item);
    if (!consumable)
        return ConsumableEffect( 0, 0, {}, {}, "" );
    int remainder = removeOne(pc, ii);
    if (remainder < 0)
        return ConsumableEffect( 0, 0, {}, {}, "" );
    return consumable->effect;
}

int inventory::removeOne(PlayerCharacter pc, InventoryItem ii) {
    map<InventoryItem, int> &inventory = currentItems[pc];
    if (!inventory.count(ii))
        return -1;
    int remaining = --inventory[ii];
    if (remaining < 1)
        inventory.erase(ii);
    return remaining;
}

int inventory::addOne(PlayerCharacter pc, InventoryItem ii) {
    map<InventoryItem, int> &inventory = currentItems[pc];
    if (inventory.count(ii) && inventory[ii] > -1)
        return ++inventory[ii];
    inventory[ii] = 1;
    return 1;
}

Item inventory::getItemData(InventoryItem itemName) {
    switch(itemName) {
        case InventoryItem::Oolong:
            return Consumable(
            InventoryItem::Oolong,
            "Oolong Tea",
            "Stimulating tea. Brewed from leaves of a carefully selected cultivar.",
                ConsumableEffect(
                    0,
                    10,
                    {},
                    {},
                    "feels a wave of calm and clarity"
                )
            );
        case InventoryItem::BeanJuice:
            return Consumable(
                InventoryItem::BeanJuice,
                "Mung Juice",
                "Juice from mature mung beans. Smells a little funky.",
                ConsumableEffect(
                    20,
                    -5,
                    { Status::drunk },
                    {},
                    "feels pretty good, but a little woozy"
                )
            );
        // TODO rest of items from Unity-Sandbox
    }
}