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

void printLocations(array<string_view, 15> names) {
    cout << "Locations:\n\n";

    for (size_t i = 0; i < names.size(); i++) {
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

bool sanitizeYesNoInput(const string output) {
    string yesNoAnswer;
    const regex yesNoRegex("^[nyNY]$");

    while ((cout << output) && (!(cin >> yesNoAnswer) || !regex_match(yesNoAnswer, yesNoRegex))) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    return toupper(yesNoAnswer[0]) == 'Y';
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

bool itemCheck(uint32_t &seed, const short location) {
    static constexpr array<short, 15> itemRateThresholds{ 25, 50, 15, 10, 25, 25, 25, 30, 20, 50, 30, 20, 25, 10, 30 };

    return getHighSeed(advanceRNG(seed)) % 100 < itemRateThresholds[location - 1];
}

bool isWantedItemCheck(uint32_t seed, const short locationThresholds, const short item) {
    static constexpr array<short, 8> itemThresholds0{ 0, 25, 45, 55, 65, 75, 95, 100 };
    static constexpr array<short, 9> itemThresholds1{ 0, 25, 45, 55, 65, 75, 85, 95, 100 };

    static constexpr array<const short*, 2> itemThresholds{ itemThresholds0.data() , itemThresholds1.data() };

    return getHighSeed(advanceRNG(seed)) % 100 >= itemThresholds[locationThresholds][item - 1] && getHighSeed(seed) % 100 < itemThresholds[locationThresholds][item];
}

void findItem(const short location, uint32_t seed, unsigned long advances, const short item) {
    for (;; advances++, advanceRNG(seed)) {
        uint32_t tempSeed = seed;

        if (getWildCheckFlag(location)) {
            if (encounterCheck(tempSeed, location)) {
                continue;
            }
        }

        if (!itemCheck(tempSeed, location)) {
            continue;
        }

        if (isWantedItemCheck(tempSeed, location == CLIFF_CAVE ? 0 : 1, item)) {
            printf("\n\nTarget seed: %08X | Target advances: %lu\n\n------------------------------------------------\n\n", seed, advances);
            return;
        }
    }
}

void findItemSeed(const short location, const short item) {
    const short hour = 24, maxDelay = 10000;
    short minDelay;
    unsigned long maxAdvances;

    sanitizeInput<short>("Insert the min delay: ", minDelay, 600, 9999);
    sanitizeInput<unsigned long>("Insert the max advances: ", maxAdvances, 1, ULONG_MAX);

    for (short ab = 0; ab < 256; ab++) {
        for (short cd = 0; cd < hour; cd++) {
            for (short efgh = minDelay; efgh < maxDelay; efgh++) {
                const uint32_t seed = ((ab << 24) | (cd << 16)) + efgh;
                uint32_t tempSeed = seed;

                for (unsigned long advances = 0; advances < maxAdvances; advances++, advanceRNG(tempSeed)) {
                    uint32_t tempSeed2 = tempSeed;

                    if (getWildCheckFlag(location)) {
                        if (encounterCheck(tempSeed2, location)) {
                            continue;
                        }
                    }

                    if (!itemCheck(tempSeed2, location)) {
                        continue;
                    }

                    if (isWantedItemCheck(tempSeed2, location == CLIFF_CAVE ? 0 : 1, item)) {
                        printf("\n\nTarget seed: %08X | Target advances: %lu\n\n------------------------------------------------\n\n", seed, advances);
                        return;
                    }
                }
            }
        }
    }
}

int main() {
    short locationIndex, itemIndex;
    string locationName;
    uint32_t initialSeed;
    unsigned long currentAdvances;
    bool knownSeedFlag;

    while (true) {
        getLocationInput(locationIndex, locationName);
        getItemInput(locationIndex, locationName, itemIndex);
        getRNGInput(initialSeed, currentAdvances, knownSeedFlag);

        if (knownSeedFlag) {
            advanceRNG(initialSeed, currentAdvances);
            findItem(locationIndex, initialSeed, currentAdvances, itemIndex);
            continue;
        }

        findItemSeed(locationIndex, itemIndex);
    }
}