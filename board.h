#define N 10
#define MAX_LINE 256
#define FLEET_COUNT 5

typedef struct {
    int cells[N][N];
} Board;

void reset_board(Board* board);
int occupied(Board* board, int r, int c);
int violates_gap_rule(Board* board, int coords[][2], int size);
void place_on_board(Board* board, int coords[][2], int size);
void write_files(Board* board);
