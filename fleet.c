#include <stdio.h>
#include "fleet.h"

void init_fleet(Fleet* fleet) {
    fleet->ships[0] = (ShipType){"Carrier", 5, 1};
    fleet->ships[1] = (ShipType){"Battleship", 4, 1};
    fleet->ships[2] = (ShipType){"Cruiser", 3, 1};
    fleet->ships[3] = (ShipType){"Submarine", 3, 1};
    fleet->ships[4] = (ShipType){"Destroyer", 2, 1};
}

void decrement_fleet_size(Fleet* fleet, int size) {
    for (int i = 0; i < FLEET_COUNT; i++) {
        if (fleet->ships[i].size == size && fleet->ships[i].count > 0) {
            fleet->ships[i].count--;
            return;
        }
    }
}

int remaining_total_ships(Fleet* fleet) {
    int total = 0;
    for (int i = 0; i < FLEET_COUNT; i++) total += fleet->ships[i].count;
    return total;
}

int fleet_has_size(Fleet* fleet, int size) {
    for (int i = 0; i < FLEET_COUNT; i++)
        if (fleet->ships[i].size == size && fleet->ships[i].count > 0) return 1;
    return 0;
}

void print_remaining(Fleet* fleet) {
    printf("Remaining ships: ");
    int first = 1;
    for (int i = 0; i < FLEET_COUNT; i++) {
        for (int k = 0; k < fleet->ships[i].count; k++) {
            if (!first) printf(", ");
            printf("%s(%d)", fleet->ships[i].name, fleet->ships[i].size);
            first = 0;
        }
    }
    if (first) printf("None");
    printf(".\n");
    printf("To enter a ship: (size) (coordinates with spaces between them, e.g.: 2 A1 A2).\n");
    printf("RULE: ships must be straight, consecutive, and must have at least 1-cell gap from other ships (no touching).\n");
}
