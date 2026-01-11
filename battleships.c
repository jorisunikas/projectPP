#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "fleet.h"
#include "board.h"

#define N 10
#define MAX_LINE 256
#define FLEET_COUNT 5


int letter_to_row(char ch);
int parse_coord(const char* s, int* out_r, int* out_c);
int in_bounds(int r, int c);
int all_same_row(int coords[][2], int size);
int all_same_col(int coords[][2], int size);
void sort_coords(int coords[][2], int size);
int has_duplicates(int coords[][2], int size);
int are_consecutive_line(int coords[][2], int size);
int try_parse_line(const char* line, int* out_size, int coords[][2]);
int validate_ship(Fleet* fleet, Board* board, int size, int coords[][2]);

int main(void) {
    Board board;
    Fleet fleet;
    
    reset_board(&board);
    init_fleet(&fleet);

    printf("Battleship ship placement (10x10)\n");
    printf("Coordinates: LETTER then NUMBER, e.g. A1, J10\n\n");

    while (remaining_total_ships(&fleet) > 0) {
        print_remaining(&fleet);
        printf("> ");

        char line[MAX_LINE];
        if (!fgets(line, sizeof(line), stdin)) {
            printf("\nInput ended.\n");
            return 0;
        }

        int size;
        int coords[10][2];
        memset(coords, 0, sizeof(coords));

        if (!try_parse_line(line, &size, coords)) {
            printf("Invalid input format. Example: 2 A1 A2\n\n");
            continue;
        }

        if (!validate_ship(&fleet, &board, size, coords)) {
            printf("Please try again.\n\n");
            continue;
        }

        place_on_board(&board, coords, size);
        decrement_fleet_size(&fleet, size);

        printf("Placed ship of size %d.\n\n", size);
    }

    printf("All ships placed! Generating website files...\n");
    write_files(&board);
    printf("Done. Open index.html in your browser.\n");
    return 0;
}



int letter_to_row(char ch) {
    ch = (char)toupper((unsigned char)ch);
    if (ch < 'A' || ch > 'J') return -1;
    return ch - 'A';
}

int parse_coord(const char* s, int* out_r, int* out_c) {
    if (!s || !*s) return 0;

    int i = 0;

    if (!isalpha((unsigned char)s[i])) return 0;
    int row = letter_to_row(s[i]);
    if (row < 0) return 0;
    i++;

    if (!isdigit((unsigned char)s[i])) return 0;
    int colNum = 0;
    while (isdigit((unsigned char)s[i])) {
        colNum = colNum * 10 + (s[i] - '0');
        i++;
    }
    if (colNum < 1 || colNum > 10) return 0;

    while (s[i] != '\0') {
        if (!isspace((unsigned char)s[i])) return 0;
        i++;
    }

    *out_r = row;
    *out_c = colNum - 1;
    return 1;
}



int all_same_row(int coords[][2], int size) {
    int r0 = coords[0][0];
    for (int i = 1; i < size; i++) if (coords[i][0] != r0) return 0;
    return 1;
}

int all_same_col(int coords[][2], int size) {
    int c0 = coords[0][1];
    for (int i = 1; i < size; i++) if (coords[i][1] != c0) return 0;
    return 1;
}

void sort_coords(int coords[][2], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size - 1; j++) {
            int r1 = coords[j][0], c1 = coords[j][1];
            int r2 = coords[j+1][0], c2 = coords[j+1][1];
            if (r1 > r2 || (r1 == r2 && c1 > c2)) {
                int tr = coords[j][0], tc = coords[j][1];
                coords[j][0] = coords[j+1][0];
                coords[j][1] = coords[j+1][1];
                coords[j+1][0] = tr;
                coords[j+1][1] = tc;
            }
        }
    }
}

int has_duplicates(int coords[][2], int size) {
    for (int i = 0; i < size; i++)
        for (int j = i + 1; j < size; j++)
            if (coords[i][0] == coords[j][0] && coords[i][1] == coords[j][1]) return 1;
    return 0;
}

int are_consecutive_line(int coords[][2], int size) {
    if (size == 1) return 1;

    int sameRow = all_same_row(coords, size);
    int sameCol = all_same_col(coords, size);
    if (!sameRow && !sameCol) return 0;

    if (sameRow) {
        for (int i = 1; i < size; i++) {
            if (coords[i][1] != coords[i-1][1] + 1) return 0;
        }
        return 1;
    } else {
        for (int i = 1; i < size; i++) {
            if (coords[i][0] != coords[i-1][0] + 1) return 0;
        }
        return 1;
    }
}


int try_parse_line(const char* line, int* out_size, int coords[][2]) {
    char buf[MAX_LINE];
    strncpy(buf, line, MAX_LINE - 1);
    buf[MAX_LINE - 1] = '\0';

    char* tok = strtok(buf, " \t\r\n");
    if (!tok) return 0;

    char* endp = NULL;
    long sizeL = strtol(tok, &endp, 10);
    if (*endp != '\0') return 0;
    if (sizeL < 1 || sizeL > 10) return 0;
    int size = (int)sizeL;

    for (int i = 0; i < size; i++) {
        tok = strtok(NULL, " \t\r\n");
        if (!tok) return 0;
        int r, c;
        if (!parse_coord(tok, &r, &c)) return 0;
        coords[i][0] = r;
        coords[i][1] = c;
    }

    tok = strtok(NULL, " \t\r\n");
    if (tok) return 0;

    *out_size = size;
    return 1;
}

int validate_ship(Fleet* fleet, Board* board, int size, int coords[][2]) {
    if (!fleet_has_size(fleet, size)) {
        printf("Invalid: no remaining ship of size %d.\n", size);
        return 0;
    }

    for (int i = 0; i < size; i++) {
        int r = coords[i][0], c = coords[i][1];
        if (!in_bounds(r, c)) {
            printf("Invalid: coordinate out of bounds.\n");
            return 0;
        }
    }

    if (has_duplicates(coords, size)) {
        printf("Invalid: duplicate coordinates.\n");
        return 0;
    }

    for (int i = 0; i < size; i++) {
        if (occupied(board, coords[i][0], coords[i][1])) {
            printf("Invalid: overlaps an existing ship.\n");
            return 0;
        }
    }

    sort_coords(coords, size);
    if (!are_consecutive_line(coords, size)) {
        printf("Invalid: ship must be straight and consecutive (no diagonal, no turns, no gaps).\n");
        return 0;
    }

    if (violates_gap_rule(board, coords, size)) {
        printf("Invalid: ships must have at least 1-cell gap (no touching sides or corners).\n");
        return 0;
    }

    return 1;
}


