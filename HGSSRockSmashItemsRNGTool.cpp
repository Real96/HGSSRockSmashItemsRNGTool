#include<iostream>
#include<math.h>
#include<climits>

using namespace std;

uint32_t LCRNG(uint32_t seed) {
    return 0x41C64E6D * seed + 0x6073;
}

void advance(uint32_t &seed, unsigned long &advances, unsigned long n = 1) {
    for (int i = 0; i < n; i++) {
        seed = LCRNG(seed);
        advances++;
    }
}

template <typename T>
void sanitizeInput(string output, T &index, T lowLimit, T highLimit) {
    while ((cout << output) && (!(cin >> index) || (index < lowLimit || index > highLimit))) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

uint16_t getHighSeed(uint32_t seed) {
    return seed >> 16;
}

bool encounterCheck(uint32_t seed, short index) {
    int encounterRateThresholds[15] = {30, 0, 0, 0, 0, 5, 0, 5, 30, 50, 20, 30, 40, 40, 20};

    return getHighSeed(seed) % 100 < encounterRateThresholds[index - 1];
}

bool itemCheck(uint32_t seed, short index) {
    int itemRateThresholds[15] = {25, 50, 15, 10, 25, 25, 25, 30, 20, 50, 30, 20, 25, 10, 30};

    return getHighSeed(seed) % 100 >= itemRateThresholds[index - 1];
}

bool isWantedItemCheck(uint32_t seed, short nameIndex, short index) {
    int* itemTresholds[2];
    itemTresholds[0] = new int[7]{25, 45, 55, 65, 75, 95, 100};
    itemTresholds[1] = new int[8]{25, 45, 55, 65, 75, 85, 95, 100};

    return getHighSeed(seed) % 100 >= itemTresholds[nameIndex][index - 2] && getHighSeed(seed) % 100 < itemTresholds[nameIndex][index - 1];
}

void findItem(uint32_t seed, bool wildFlag, short location, unsigned long advances, short itemTresholdIndex, short itemIndex) {
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

        if (isWantedItemCheck(tempSeed, itemTresholdIndex, itemIndex)) {
            printf("\n\nTarget seed: %08X | Target advances: %lu\n\n\n", seed, advances);
            break;
        }

        advance(seed, advances);
    }
}

int main() {
    cout << "1  Cliff Cave\n2  Violet City\n3  Tohjo Falls\n4  Route 3\n5  Mt. Silver\n6  Cerulean Cave 1F\n7  Cerulean Cave 2F\n8  Cerulean Cave B1F\n"
        << "9  Cianwood City\n10 Dark Cave\n11 Rock Tunnel\n12 Route 19\n13 Vemillion City\n14 Victory Road\n15 Ruins of Alph\n\n";
    string* itemNames[3];
    itemNames[0] = new string[7]{"Max Ether", "Pearl", "Big Pearl", "Red Shard (HG) / Blue Shard (SS)", "Yellow Shard (HG) / Green Shard (SS)",
                            "Claw Fossil (HG) / Root Fossil (SS)", "Rare Bone"};
    itemNames[1] = new string[8]{"Max Ether", "Revive", "Heart Scale", "Red Shard", "Blue Shard", "Green Shard", "Yellow Shard", "Star Piece"};
    itemNames[2] = new string[8]{"Red Shard", "Yellow Shard", "Helix Fossil (HG) / Dome Fossil (SS)", "Max Ether", "Blue Shard", "Green Shard", "Old Amber", "Max Revive"};
    short itemNameIndex, itemsTotalNumber, itemTresholdIndex, location, itemIndex;
    uint32_t currentSeed;
    bool wildEncounterCheck;
    unsigned long advances, currentAdvances;

    while (true) {
        itemNameIndex = 1, itemsTotalNumber = 8, itemTresholdIndex = 1, wildEncounterCheck = true, advances = 0;

        sanitizeInput<short>("Insert the location number: ", location, 1, 15);

        switch (location) {
            case 1:
                itemNameIndex = 0;
                itemsTotalNumber = 7;
                itemTresholdIndex = 0;
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

        cout << "\nInsert the initial seed: ";
        scanf("%X", &currentSeed);

        cout << "\n";
        sanitizeInput<unsigned long>("Insert the current advances: ", currentAdvances, 0, ULONG_MAX);

        advance(currentSeed, advances, currentAdvances);
        findItem(currentSeed, wildEncounterCheck, location, advances, itemTresholdIndex, itemIndex);
    }
}