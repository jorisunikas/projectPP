#define N 10
#define MAX_LINE 256
#define FLEET_COUNT 5

typedef struct {
    const char* name;
    int size;
    int count;
} ShipType;

typedef struct {
    ShipType ships[FLEET_COUNT];
} Fleet;


void init_fleet(Fleet* fleet);
void decrement_fleet_size(Fleet* fleet, int size);
int remaining_total_ships(Fleet* fleet);
int fleet_has_size(Fleet* fleet, int size);
void print_remaining(Fleet* fleet);
