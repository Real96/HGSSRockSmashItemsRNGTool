#include <iostream>
#include <math.h>
#include <climits>
#include <string>
#include <string_view>
#include <array>
#include <regex>

using namespace std;

uint32_t LCRNG(uint32_t seed) {
    return 0x41C64E6D * seed + 0x6073;
}

void advance(uint32_t &seed, unsigned long &advances, unsigned long n = 1) {
    for (unsigned long i = 0; i < n; i++) {
        seed = LCRNG(seed);
        advances++;
    }
}

template <typename T>
void sanitizeInput(const string &output, T &index, T lowLimit, T highLimit) {
    while ((cout << output) && (!(cin >> index) || (index < lowLimit || index > highLimit))) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void sanitizeHexInput(uint32_t &seed) {
    string stringSeed;
    regex hexRegex("^[0-9a-fA-F]{1,8}$");

    while ((cout << "Insert the initial seed: ") && (!(cin >> stringSeed) || !regex_match(stringSeed, hexRegex))) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    char* endPtr;
    seed = strtoul(stringSeed.c_str(), &endPtr, 16);
}

uint16_t getHighSeed(uint32_t seed) {
    return seed >> 16;
}

bool encounterCheck(uint32_t seed, short index) {
    static constexpr array<int, 15> encounterRateThresholds{ 30, 0, 0, 0, 0, 5, 0, 5, 30, 50, 20, 30, 40, 40, 20 };

    return getHighSeed(seed) % 100 < encounterRateThresholds[index - 1];
}

bool itemCheck(uint32_t seed, short index) {
    static constexpr array<int, 15> itemRateThresholds{ 25, 50, 15, 10, 25, 25, 25, 30, 20, 50, 30, 20, 25, 10, 30 };

    return getHighSeed(seed) % 100 >= itemRateThresholds[index - 1];
}

bool isWantedItemCheck(uint32_t seed, short nameIndex, short index) {
    
    static constexpr array<int, 7> itemThresholds0{ 25, 45, 55, 65, 75, 95, 100 };
    static constexpr array<int, 8> itemThresholds1{ 25, 45, 55, 65, 75, 85, 95, 100 };

    static constexpr array<const int*, 2> itemThresholds{ itemThresholds0.data() , itemThresholds1.data() };

    return getHighSeed(seed) % 100 >= itemThresholds[nameIndex][index - 2] && getHighSeed(seed) % 100 < itemThresholds[nameIndex][index - 1];
}

void findItem(uint32_t seed, bool wildFlag, short location, unsigned long advances, short itemThresholdIndex, short itemIndex) {
    uint32_t tempSeed;

    while (true) {
        tempSeed = seed;

        if (wildFlag) {
            tempSeed = LCRNG(tempSeed);

            if (encounterCheck(tempSeed, location)) {
                advance(seed, advances);
                continue;
            }
        }

        tempSeed = LCRNG(tempSeed);

        if (itemCheck(tempSeed, location)) {
            advance(seed, advances);
            continue;
        }

        tempSeed = LCRNG(tempSeed);

        if (isWantedItemCheck(tempSeed, itemThresholdIndex, itemIndex)) {
            printf("\n\nTarget seed: %08X | Target advances: %lu\n\n\n", seed, advances);
            break;
        }

        advance(seed, advances);
    }
}

int main() {
    cout << "1  Cliff Cave\n2  Violet City\n3  Tohjo Falls\n4  Route 3\n5  Mt. Silver\n6  Cerulean Cave 1F\n7  Cerulean Cave 2F\n8  Cerulean Cave B1F\n"
        << "9  Cianwood City\n10 Dark Cave\n11 Rock Tunnel\n12 Route 19\n13 Vemillion City\n14 Victory Road\n15 Ruins of Alph\n\n";
  
    static constexpr array<string_view,7> itemNames0{ "Max Ether", "Pearl", "Big Pearl", "Red Shard (HG) / Blue Shard (SS)", "Yellow Shard (HG) / Green Shard (SS)",
                            "Claw Fossil (HG) / Root Fossil (SS)", "Rare Bone" };
    static constexpr array<string_view,8> itemNames1{ "Max Ether", "Revive", "Heart Scale", "Red Shard", "Blue Shard", "Green Shard", "Yellow Shard", "Star Piece" };
    static constexpr array<string_view,8> itemNames2{ "Red Shard", "Yellow Shard", "Helix Fossil (HG) / Dome Fossil (SS)", "Max Ether", "Blue Shard", "Green Shard", "Old Amber", "Max Revive" };

    static constexpr array<const string_view*, 3> itemNames{ itemNames0.data(), itemNames1.data(), itemNames2.data() };
    short itemNameIndex, itemsTotalNumber, itemThresholdIndex, location, itemIndex;
    uint32_t currentSeed;
    bool wildEncounterCheck;
    unsigned long advances, currentAdvances;

    while (true) {
        itemNameIndex = 1, itemsTotalNumber = 8, itemThresholdIndex = 1, wildEncounterCheck = true, advances = 0;

        sanitizeInput<short>("Insert the location number: ", location, 1, 15);

        switch (location) {
            case 1:
                itemNameIndex = 0;
                itemsTotalNumber = 7;
                itemThresholdIndex = 0;
                break;
            case 2:
            case 3:
            case 4:
            case 5:
            case 7:
                wildEncounterCheck = false;
                break;
            case 15:
                itemNameIndex = 2;
                break;
        }

        for (int i = 0; i < itemsTotalNumber; i++) {
            cout << "\n" << i + 1 << " " << itemNames[itemNameIndex][i];
        }

        cout << "\n\n";
        sanitizeInput<short>("Insert the wanted item number: ", itemIndex, 1, itemsTotalNumber);

        cout << "\n";
        sanitizeHexInput(currentSeed);

        cout << "\n";
        sanitizeInput<unsigned long>("Insert the current advances: ", currentAdvances, 0, ULONG_MAX);

        advance(currentSeed, advances, currentAdvances);
        findItem(currentSeed, wildEncounterCheck, location, advances, itemThresholdIndex, itemIndex);
    }
}