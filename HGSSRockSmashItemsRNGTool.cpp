#include <iostream>
#include <math.h>
#include <limits>
#include <string>
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
void sanitizeInput(const string &output, T &index, T lowLimit, T highLimit) {
    while ((cout << output) && (!(cin >> index) || (index < lowLimit || index > highLimit))) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void getLocationInput(short &index, string &name) {
    static constexpr array<string_view, 15> locationNames{ "Cliff Cave", "Violet City", "Tohjo Falls", "Route 3", "Mt. Silver", "Cerulean Cave 1F", "Cerulean Cave 2F", "Cerulean Cave B1F",
                                                        "Cianwood City", "Dark Cave", "Rock Tunnel", "Route 19", "Vermilion City", "Victory Road", "Ruins of Alph" };

    printLocations(locationNames);
    sanitizeInput<short>("Insert the location number: ", index, 1, 15);
    cout << "\n\n";
    name = locationNames[index - 1];
}

void printItemsName(short locationIndex, string locationName) {
    static constexpr array itemNames0{ to_array<string_view>({ "Max Ether", "Pearl", "Big Pearl", "Red Shard (HG) / Blue Shard (SS)", "Yellow Shard (HG) / Green Shard (SS)",
                                                            "Claw Fossil (HG) / Root Fossil (SS)", "Rare Bone" }) };
    static constexpr array itemNames1{ to_array<string_view>({ "Max Ether", "Revive", "Heart Scale", "Red Shard", "Blue Shard", "Green Shard", "Yellow Shard", "Star Piece" }) };
    static constexpr array itemNames2{ to_array<string_view>({ "Red Shard", "Yellow Shard", "Helix Fossil (HG) / Dome Fossil (SS)", "Max Ether", "Blue Shard", "Green Shard",
                                                            "Old Amber", "Max Revive" }) };

    using strview_span = span<const string_view>;

    static constexpr array itemNames{ to_array<strview_span>({
        strview_span(itemNames0),
        strview_span(itemNames1),
        strview_span(itemNames2)
    }) };

    short itemsGroupIndex = locationIndex == CLIFF_CAVE ? 0 : locationIndex == RUINS_OF_ALPH ? 2 : 1;
    strview_span itemGroupNames = itemNames[itemsGroupIndex];

    cout << locationName << " items:\n\n";

    for (size_t i = 0; i < itemGroupNames.size(); i++) {
        cout << i + 1 << " " << itemGroupNames[i] << "\n";
    }

     cout << "\n\n";
}

void getItemInput(short locationIndex, string locationName, short &itemIndex) {
    printItemsName(locationIndex, locationName);
    sanitizeInput<short>("Insert the wanted item number: ", itemIndex, 1, locationIndex == CLIFF_CAVE ? 7 : 8);
}

void sanitizeHexInput(uint32_t &seed) {
    string stringSeed;
    regex hexRegex("^[0-9a-fA-F]{1,8}$");

    while ((cout << "Insert the initial seed: ") && (!(cin >> stringSeed) || !regex_match(stringSeed, hexRegex))) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    seed = stoul(stringSeed, nullptr, 16);
}

uint32_t LCRNG(uint32_t seed) {
    return 0x41C64E6D * seed + 0x6073;
}

void advance(uint32_t &seed, unsigned long &advances, unsigned long n = 1) {
    for (unsigned long i = 0; i < n; i++) {
        seed = LCRNG(seed);
        advances++;
    }
}

bool getWildCheckFlag(short locationIndex) {
    switch (locationIndex) {
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

uint16_t getHighSeed(uint32_t seed) {
    return seed >> 16;
}

bool encounterCheck(uint32_t seed, short index) {
    static constexpr array<short, 15> encounterRateThresholds{ 30, 0, 0, 0, 0, 5, 0, 5, 30, 50, 20, 30, 40, 40, 20 };

    return getHighSeed(seed) % 100 < encounterRateThresholds[index - 1];
}

bool itemCheck(uint32_t seed, short index) {
    static constexpr array<short, 15> itemRateThresholds{ 25, 50, 15, 10, 25, 25, 25, 30, 20, 50, 30, 20, 25, 10, 30 };

    return getHighSeed(seed) % 100 >= itemRateThresholds[index - 1];
}

bool isWantedItemCheck(uint32_t seed, short thresholdsGroupIndex, short index) {
    static constexpr array<short, 8> itemThresholds0{ 0, 25, 45, 55, 65, 75, 95, 100 };
    static constexpr array<short, 9> itemThresholds1{ 0, 25, 45, 55, 65, 75, 85, 95, 100 };

    static constexpr array<const short*, 2> itemThresholds{ itemThresholds0.data() , itemThresholds1.data() };

    return getHighSeed(seed) % 100 >= itemThresholds[thresholdsGroupIndex][index - 1] && getHighSeed(seed) % 100 < itemThresholds[thresholdsGroupIndex][index];
}

void findItem(short locationIndex, uint32_t seed, unsigned long advances, short itemIndex) {
    uint32_t tempSeed;
    bool wildCheckFlag = getWildCheckFlag(locationIndex);
    short itemsThresholdsGroupIndex = locationIndex == CLIFF_CAVE ? 0 : 1;

    while (true) {
        tempSeed = seed;

        if (wildCheckFlag) {
            tempSeed = LCRNG(tempSeed);

            if (encounterCheck(tempSeed, locationIndex)) {
                advance(seed, advances);
                continue;
            }
        }

        tempSeed = LCRNG(tempSeed);

        if (itemCheck(tempSeed, locationIndex)) {
            advance(seed, advances);
            continue;
        }

        tempSeed = LCRNG(tempSeed);

        if (isWantedItemCheck(tempSeed, itemsThresholdsGroupIndex, itemIndex)) {
            printf("\n\nTarget seed: %08X | Target advances: %lu\n\n------------------------------------------------\n\n", seed, advances);
            break;
        }

        advance(seed, advances);
    }
}

int main() {
    short locationIndex, itemIndex;
    string locationName;
    uint32_t currentSeed;
    unsigned long currentAdvances, advances;

    while (true) {
        getLocationInput(locationIndex, locationName);
        getItemInput(locationIndex, locationName, itemIndex);
        sanitizeHexInput(currentSeed);
        sanitizeInput<unsigned long>("Insert the current advances: ", currentAdvances, 0, ULONG_MAX);

        advances = 0;
        advance(currentSeed, advances, currentAdvances);
        findItem(locationIndex, currentSeed, advances, itemIndex);
    }
}