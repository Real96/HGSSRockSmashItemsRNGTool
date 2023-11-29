#include<iostream>
#include <math.h>

using namespace std;

uint32_t LCRNG(uint32_t seed) {
    return 0x41C64E6D * seed + 0x6073;
}

uint16_t getHighSeed(uint32_t seed) {
    return seed >> 16;
}

int main() {
    int encounterRateThresholds[15] = {30, 0, 0, 0, 0, 5, 0, 5, 30, 50, 20, 30, 40, 40, 20};
    int itemRateThresholds[15] = {25, 50, 15, 10, 25, 25, 25, 30, 20, 50, 30, 20, 25, 10, 30};

    string* itemNames[3];
    itemNames[0] = new string[7]{"Max Ether", "Pearl", "Big Pearl", "Red Shard (HG) / Blue Shard (SS)", "Yellow Shard (HG) / Green Shard (SS)",
                            "Claw Fossil (HG) / Root Fossil (SS)", "Rare Bone"};
    itemNames[1] = new string[8]{"Max Ether", "Revive", "Heart Scale", "Red Shard", "Blue Shard", "Green Shard", "Yellow Shard", "Star Piece"};
    itemNames[2] = new string[8]{"Red Shard", "Yellow Shard", "Helix Fossil (HG) / Dome Fossil (SS)", "Max Ether", "Blue Shard", "Green Shard", "Old Amber", "Max Revive"};

    int* itemTresholds[2];
    itemTresholds[0] = new int[8]{0, 25, 45, 55, 65, 75, 95, 100};
    itemTresholds[1] = new int[9]{0, 25, 45, 55, 65, 75, 85, 95, 100};

    cout << "1  Cliff Cave\n2  Violet City\n3  Tohjo Falls\n4  Route 3\n5  Mt. Silver\n6  Cerulean Cave 1F\n7  Cerulean Cave 2F\n8  Cerulean Cave B1F\n"
        << "9  Cianwood City\n10 Dark Cave\n11 Rock Tunnel\n12 Route 19\n13 Vemillion City\n14 Victory Road\n15 Ruins of Alph\n\n";

    while (true) {
        short location;
        uint32_t seed, tempSeed;
        unsigned long advances = 0, tempAdvances;
        short itemNamesIndex = 1, itemsTotalNumber = 8, itemIndex;
        bool wildEncounterCheck = true, itemFound = false;

        do {
            cout << "Insert the location number: ";
            cin >> location;
        } while (location < 1 || location > 15);

        switch (location) {
            case 1:
                itemNamesIndex = 0;
                itemsTotalNumber = 7;
                break;
            case 2:
            case 3:
            case 4:
            case 5:
            case 7:
                wildEncounterCheck = false;
                break;
            case 15:
                itemNamesIndex = 2;
                break;
        }

        for (int i = 0; i < itemsTotalNumber; i++) {
            cout << endl << i + 1 << " " << itemNames[itemNamesIndex][i];
        }

        cout << endl << endl;

        do {
            cout << "Insert the wanted item number: ";
            cin >> itemIndex;
        } while (itemIndex < 1 || itemIndex > itemsTotalNumber);

        cout << endl << "Insert the Initial Seed: ";
        scanf("%X", &seed);

        cout << endl << "Insert the Starting Advances: ";
        cin >> advances;

        for (int i = 0; i < advances; i++) {
            seed = LCRNG(seed);
        }

        while (!itemFound) {
            tempSeed = seed;
            tempAdvances = advances;

            if (wildEncounterCheck) {
                tempSeed = LCRNG(tempSeed);
                tempAdvances++;

                if (getHighSeed(tempSeed) % 100 < encounterRateThresholds[location - 1]) {
                    seed = LCRNG(seed);
                    advances++;
                    continue;
                }
            }

            tempSeed = LCRNG(tempSeed);
            tempAdvances++;

            if (getHighSeed(tempSeed) % 100 >= itemRateThresholds[location - 1]) {
                seed = LCRNG(seed);
                advances++;
                continue;
            }

            tempSeed = LCRNG(tempSeed);
            tempAdvances++;

            if (getHighSeed(tempSeed) % 100 >= itemTresholds[itemNamesIndex][itemIndex - 1] && getHighSeed(tempSeed) % 100 < itemTresholds[itemNamesIndex][itemIndex]) {
                itemFound = true;
                printf("\n\nCurrent Seed: %08X | Advances: %lu\n\n", seed, advances);
            }
            else {
                seed = LCRNG(seed);
                advances++;
            }
        }
    }
}
