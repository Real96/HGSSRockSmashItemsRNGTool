#include <iostream>
#include <math.h>
#include <string_view>
#include <array>
#include <regex>
#include <span>

using namespace std;

enum Location {
    CLIFF_CAVE = 1,
    VIOLET_CITY = 2,
    TOHJO_FALLS = 3,
    ROUTE_3 = 4,
    MT_SILVER = 5,
    CERULEAN_CAVE_1F = 6,
    CERULEAN_CAVE_2F = 7,
    CERULEAN_CAVE_B1F = 8,
    CIANWOOD_CITY = 9,
    DARK_CAVE = 10,
    ROCK_TUNNEL = 11,
    ROUTE_19 = 12,
    VERMILION_CITY = 13,
    VICTORY_ROAD = 14,
    RUINS_OF_ALPH = 15
};

enum Ability {
    NONE = 1,
    SUCTION_CUPS = 2,
    MAGNET_PULL = 3,
    KEEN_EYE = 4,
    SERENE_GRACE = 5,
    SUPER_LUCK = 6
};

void printLocations(array<string_view, 15> names) {
    cout << "Locations:\n\n";

    for (size_t i = 0; i < 15; i++) {
        cout << i + 1 << " " << names[i] << "\n";
    }

    cout << "\n\n";
}

template <typename T>
void sanitizeInput(const string output, T &value, const T lowLimit, const T highLimit) {
    while ((cout << output) && (!(cin >> value) || (value < lowLimit || value > highLimit))) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void getLocationInput(short &location, string &name) {
    static constexpr array<string_view, 15> locationNames{ "Cliff Cave", "Violet City", "Tohjo Falls", "Route 3", "Mt. Silver", "Cerulean Cave 1F", "Cerulean Cave 2F", "Cerulean Cave B1F",
                                                           "Cianwood City", "Dark Cave", "Rock Tunnel", "Route 19", "Vermilion City", "Victory Road", "Ruins of Alph" };

    printLocations(locationNames);
    sanitizeInput<short>("Insert the location number: ", location, 1, 15);
    cout << "\n\n";
    name = locationNames[location - 1];
}

void printItemsName(const short location) {
    static constexpr array itemNames0{ to_array<string_view>({ "Max Ether", "Pearl", "Big Pearl", "Red Shard (HG) / Blue Shard (SS)", "Yellow Shard (HG) / Green Shard (SS)",
                                                               "Claw Fossil (HG) / Root Fossil (SS)", "Rare Bone" }) };
    static constexpr array itemNames1{ to_array<string_view>({ "Max Ether", "Revive", "Heart Scale", "Red Shard", "Blue Shard", "Green Shard", "Yellow Shard", "Star Piece" }) };
    static constexpr array itemNames2{ to_array<string_view>({ "Red Shard (HG) / Blue Shard (SS)", "Yellow Shard (HG) / Green Shard (SS)", "Helix Fossil (HG) / Dome Fossil (SS)",
                                                               "Max Ether", "Blue Shard (HG) / Red Shard (SS)", "Green Shard (HG) / Yellow Shard (SS)", "Old Amber", "Max Revive" }) };

    using strview_span = span<const string_view>;

    static constexpr array itemNames{ to_array<strview_span>({
        strview_span(itemNames0),
        strview_span(itemNames1),
        strview_span(itemNames2)
    }) };

    const short itemsGroupIndex = location == CLIFF_CAVE ? 0 : location == RUINS_OF_ALPH ? 2 : 1;
    strview_span itemGroupNames = itemNames[itemsGroupIndex];

    for (size_t i = 0; i < itemGroupNames.size(); i++) {
        cout << i + 1 << " " << itemGroupNames[i] << "\n";
    }

    cout << "\n\n";
}

void getItemInput(const short location, const string locationName, short &item) {
    cout << locationName << " items:\n\n";
    printItemsName(location);
    sanitizeInput<short>("Insert the wanted item number: ", item, 1, location == CLIFF_CAVE ? 7 : 8);
    cout << "\n\n";
}

void printLeadAbilities() {
    static constexpr array<string_view, 6> abilityNames{ "None", "Suction Cups", "Magnet Pull", "Keen Eye", "Serene Grace", "Super Luck" };
    cout << "Abilities:\n\n";

    for (size_t i = 0; i < 6; i++) {
        cout << i + 1 << " " << abilityNames[i] << "\n";
    }

    cout << "\n\n";
}

void getAbilityInput(short &ability) {
    printLeadAbilities();
    sanitizeInput<short>("Insert  number of the ability of your party leader pokemon: ", ability, 1, 6);
}

bool sanitizeYesNoInput(const string output) {
    string yesNoAnswer;
    const regex yesNoRegex("^[nyNY]$");

    while ((cout << output) && (!(cin >> yesNoAnswer) || !regex_match(yesNoAnswer, yesNoRegex))) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    return toupper(yesNoAnswer[0]) == 'Y';
}

void getLeadRockSmashInput(bool &leadUsingRockSmash) {
    leadUsingRockSmash = sanitizeYesNoInput("Is your party leader pokemon using RockSmash HM? (y/n) ");
}

void setThresholdIncreaser(short &thresholdIncreaser, const short ability, const bool leadUsingRockSmash) {
    switch (ability) {
    case SUCTION_CUPS:
    case MAGNET_PULL:
    case KEEN_EYE:
        thresholdIncreaser = 5;
        break;
    default:
        thresholdIncreaser = 0;
        break;
    }

    if (leadUsingRockSmash) {
        thresholdIncreaser += 5;
    }
}

void setItemIndexIncreaser(short &itemIncreaser, const short location, const short item, short const ability) {
    switch(ability) {
        case SERENE_GRACE:
        case SUPER_LUCK:
            itemIncreaser = 1;
            break;
        default:
            itemIncreaser = 0;
            break;
    }
}

void sanitizeHexInput(uint32_t &seed) {
    string stringSeed;
    const regex hexRegex("^[0-9a-fA-F]{1,8}$");

    while ((cout << "Insert the initial seed: ") && (!(cin >> stringSeed) || !regex_match(stringSeed, hexRegex))) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    seed = stoul(stringSeed, nullptr, 16);
}

void getRNGInput(uint32_t &seed, unsigned long &advances, bool &knownSeedFlag) {
    knownSeedFlag = sanitizeYesNoInput("Do you know the initial seed? (y/n) ");

    if (knownSeedFlag) {
        sanitizeHexInput(seed);
        sanitizeInput<unsigned long>("Insert the current advances: ", advances, 0, ULONG_MAX);
    }
}

bool getWildCheckFlag(short location) {
    switch (location) {
        case VIOLET_CITY:
        case TOHJO_FALLS:
        case ROUTE_3:
        case MT_SILVER:
        case CERULEAN_CAVE_2F:
            return false;
        default:
            return true;
    }
}

uint16_t getHighSeed(const uint32_t seed) {
    return seed >> 16;
}

uint32_t LCRNG(const uint32_t seed) {
    return 0x41C64E6D * seed + 0x6073;
}

uint32_t advanceRNG(uint32_t &seed, const unsigned long n = 1) {
    for (unsigned long i = 0; i < n; i++) {
        seed = LCRNG(seed);
    }

    return seed;
}

bool encounterCheck(uint32_t &seed, const short location) {
    static constexpr array<short, 15> encounterRateThresholds{ 30, 0, 0, 0, 0, 5, 0, 5, 30, 50, 20, 30, 40, 40, 20 };

    return getHighSeed(advanceRNG(seed)) % 100 < encounterRateThresholds[location - 1];
}

bool itemCheck(uint32_t &seed, const short location, const short thresholdIncreaser) {
    static constexpr array<short, 15> itemRateThresholds{ 25, 50, 15, 10, 25, 25, 25, 30, 20, 50, 30, 20, 25, 10, 30 };

    return getHighSeed(advanceRNG(seed)) % 100 < itemRateThresholds[location - 1] + thresholdIncreaser;
}

short getGeneratedItem(const short locationThresholds, uint32_t seed, short itemIncreaser) {
    static constexpr array itemThresholds0{ to_array<short>({ 25, 45, 55, 65, 75, 95, 100 }) };
    static constexpr array itemThresholds1{ to_array<short>({ 25, 45, 55, 65, 75, 85, 95, 100 }) };

    using strview_span = span<const short>;

    static constexpr array itemThresholds{ to_array<strview_span>({
        span<const short>( itemThresholds0 ),
        span<const short>( itemThresholds1 )
    }) };

    strview_span itemGroupTresholds = itemThresholds[locationThresholds];

    for (short item = 0; item < itemGroupTresholds.size(); item++) {
        uint16_t highSeed = getHighSeed(seed) % 100;

        if (highSeed < itemGroupTresholds[item]) {
            if (locationThresholds == 0 && item == 5 && highSeed < 85) {
                itemIncreaser = 0;
            }
            else if (itemIncreaser) {
                itemIncreaser = locationThresholds == 0 ? (item + 1 == 7 ? 0 : 1) : (item + 1 == 8 ? 0 : 1);
            }

            return (item + 1) + itemIncreaser;
        }
    }
}

bool isWantedItemCheck(uint32_t seed, const short locationThresholds, const short item, short itemIncreaser) {
    return getGeneratedItem(locationThresholds, advanceRNG(seed), itemIncreaser) == item;
}

void findItem(const short location, uint32_t seed, unsigned long advances, const short item, const short thresholdIncreaser, const short itemIndexIncreaser) {
    for (;; advances++, advanceRNG(seed)) {
        uint32_t tempSeed = seed;

        if (getWildCheckFlag(location)) {
            if (encounterCheck(tempSeed, location)) {
                continue;
            }
        }

        if (!itemCheck(tempSeed, location, thresholdIncreaser)) {
            continue;
        }

        if (item == 1 && itemIndexIncreaser) {
            printf("\n\nYou cannot get the slot 1 item with ability Serene Grace / Super Luck");
            return;
        }

        if (isWantedItemCheck(tempSeed, location == CLIFF_CAVE ? 0 : 1, item, itemIndexIncreaser)) {
            printf("\n\nTarget seed: %08X | Target advances: %lu", seed, advances);
            return;
        }
    }
}

void findItemSeed(const short location, const short item, const short thresholdIncreaser, const short itemIndexIncreaser) {
    const short hour = 24, maxDelay = 10000;
    short minDelay;
    unsigned long maxAdvances;

    sanitizeInput<short>("Insert the min delay: ", minDelay, 600, 9999);
    sanitizeInput<unsigned long>("Insert the max advances: ", maxAdvances, 0, ULONG_MAX);

    for (short ab = 0; ab < 256; ab++) {
        for (short cd = 0; cd < hour; cd++) {
            for (short efgh = minDelay; efgh < maxDelay; efgh++) {
                const uint32_t seed = ((ab << 24) | (cd << 16)) + efgh;
                uint32_t tempSeed = seed;

                for (unsigned long advances = 0; advances <= maxAdvances; advances++, advanceRNG(tempSeed)) {
                    uint32_t tempSeed2 = tempSeed;

                    if (getWildCheckFlag(location)) {
                        if (encounterCheck(tempSeed2, location)) {
                            continue;
                        }
                    }

                    if (!itemCheck(tempSeed2, location, thresholdIncreaser)) {
                        continue;
                    }

                    if (item == 1 && itemIndexIncreaser) {
                        printf("\n\nYou cannot get the slot 1 item with ability Serene Grace / Super Luck");
                        return;
                    }

                    if (isWantedItemCheck(tempSeed2, location == CLIFF_CAVE ? 0 : 1, item, itemIndexIncreaser)) {
                        printf("\n\nTarget seed: %08X | Target advances: %lu", seed, advances);
                        return;
                    }
                }
            }
        }
    }
}

int main() {
    short locationIndex, itemIndex, ability, thresholdIncreaser, itemIndexIncreaser;
    string locationName;
    uint32_t initialSeed;
    unsigned long currentAdvances;
    bool leadUsingRockSmash, knownSeedFlag;

    while (true) {
        getLocationInput(locationIndex, locationName);
        getItemInput(locationIndex, locationName, itemIndex);
        getAbilityInput(ability);
        getLeadRockSmashInput(leadUsingRockSmash);
        setThresholdIncreaser(thresholdIncreaser, ability, leadUsingRockSmash);
        setItemIndexIncreaser(itemIndexIncreaser, locationIndex, itemIndex, ability);
        getRNGInput(initialSeed, currentAdvances, knownSeedFlag);

        if (knownSeedFlag) {
            advanceRNG(initialSeed, currentAdvances);
            findItem(locationIndex, initialSeed, currentAdvances, itemIndex, thresholdIncreaser, itemIndexIncreaser);
            printf("\n\n------------------------------------------------\n\n");
            continue;
        }

        findItemSeed(locationIndex, itemIndex, thresholdIncreaser, itemIndexIncreaser);
        printf("\n\n------------------------------------------------\n\n");
    }
}