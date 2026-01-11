#include "board.h"
#include <stdio.h>
#include <stdlib.h>

void reset_board(Board* board) {
    for (int r = 0; r < N; r++)
        for (int c = 0; c < N; c++)
            board->cells[r][c] = 0;
}

int occupied(Board* board, int r, int c) {
    return board->cells[r][c] == 1;
}

int in_bounds(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < N;
}


int violates_gap_rule(Board* board, int coords[][2], int size) {
    const int dr[8] = {-1,-1,-1, 0,0, 1,1,1};
    const int dc[8] = {-1, 0, 1,-1,1,-1,0,1};

    for (int i = 0; i < size; i++) {
        int r = coords[i][0], c = coords[i][1];

        for (int k = 0; k < 8; k++) {
            int nr = r + dr[k];
            int nc = c + dc[k];
            if (!in_bounds(nr, nc)) continue;
            if (board->cells[nr][nc] == 1) return 1;
        }
    }
    return 0;
}

void place_on_board(Board* board, int coords[][2], int size) {
    for (int i = 0; i < size; i++) board->cells[coords[i][0]][coords[i][1]] = 1;
}

void write_files(Board* board) {
    FILE* f = fopen("index.html", "w");
    if (!f) { perror("index.html"); exit(1); }

    fprintf(f,
        "<!doctype html>\n"
        "<html lang=\"lt\">\n"
        "<head>\n"
        "  <meta charset=\"utf-8\" />\n"
        "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />\n"
        "  <title>Laivų mūšis</title>\n"
        "  <link rel=\"stylesheet\" href=\"style.css\" />\n"
        "</head>\n"
        "<body>\n"
        "  <header><h1>Laivų mūšis</h1></header>\n"
        "  <main>\n"
        "    <aside class=\"sidebar\">\n"
        "      <h3>Your ships</h3>\n"
        "      <div class=\"ship\">Carrier (5)</div>\n"
        "      <div class=\"ship\">Battleship (4)</div>\n"
        "      <div class=\"ship\">Cruiser (3)</div>\n"
        "      <div class=\"ship\">Submarine (3)</div>\n"
        "      <div class=\"ship\">Destroyer (2)</div>\n"
        "      <hr class=\"side-sep\" />\n"
        "      <h3>Enemy ships</h3>\n"
        "      <div class=\"ship\">Carrier (5)</div>\n"
        "      <div class=\"ship\">Battleship (4)</div>\n"
        "      <div class=\"ship\">Cruiser (3)</div>\n"
        "      <div class=\"ship\">Submarine (3)</div>\n"
        "      <div class=\"ship\">Destroyer (2)</div>\n"
        "    </aside>\n"
        "\n"
        "    <section class=\"boards-section\">\n"
        "      <div class=\"board-container\">\n"
        "        <h2>Player</h2>\n"
        "        <div id=\"player-board\" class=\"board-wrapper\"></div>\n"
        "      </div>\n"
        "      <div class=\"board-container\">\n"
        "        <h2>Enemy</h2>\n"
        "        <div id=\"enemy-board\" class=\"board-wrapper\"></div>\n"
        "      </div>\n"
        "    </section>\n"
        "  </main>\n"
        "  <script src=\"script.js\"></script>\n"
        "</body>\n"
        "</html>\n"
    );
    fclose(f);

    f = fopen("style.css", "w");
    if (!f) { perror("style.css"); exit(1); }

    fprintf(f,
        "html { height: 100%%; }\n"
        "body {\n"
        "  display: flex;\n"
        "  flex-direction: column;\n"
        "  margin: 0;\n"
        "  font-family: Arial, sans-serif;\n"
        "  color: #222;\n"
        "  height: 100%%;\n"
        "}\n"
        "main {\n"
        "  display: flex;\n"
        "  flex-direction: row;\n"
        "  flex: 1;\n"
        "  height: 100%%;\n"
        "}\n"
        ".sidebar {\n"
        "  width: 20%%;\n"
        "  text-align: center;\n"
        "  background-color: #023047;\n"
        "  color: #ffb703;\n"
        "  font-weight: bold;\n"
        "  padding: 20px 10px;\n"
        "  box-sizing: border-box;\n"
        "}\n"
        ".side-sep {\n"
        "  border: none;\n"
        "  height: 1px;\n"
        "  background: rgba(255,183,3,0.35);\n"
        "  margin: 16px 0;\n"
        "}\n"
        ".ship { margin-bottom: 10px; color: #fb8500; }\n"
        "\n"
        ".boards-section {\n"
        "  display: flex;\n"
        "  flex-direction: row;\n"
        "  text-align: center;\n"
        "  width: 80%%;\n"
        "  height: 100%%;\n"
        "}\n"
        ".board-container {\n"
        "  width: 50%%;\n"
        "  height: 100%%;\n"
        "  background: #8ecae6;\n"
        "}\n"
        "header {\n"
        "  text-align: center;\n"
        "  height: 80px;\n"
        "  background-color: #023047;\n"
        "  color: #ffb703;\n"
        "}\n"
        ".board-wrapper {\n"
        "  display: grid;\n"
        "  grid-template-columns: 30px repeat(10, 30px);\n"
        "  grid-template-rows: 30px repeat(10, 30px);\n"
        "  justify-content: center;\n"
        "  margin: 0 auto;\n"
        "  background: #8ecae6;\n"
        "  box-sizing: border-box;\n"
        "}\n"
        ".coord {\n"
        "  display: flex;\n"
        "  align-items: center;\n"
        "  justify-content: center;\n"
        "  font-weight: bold;\n"
        "}\n"
        ".cell {\n"
        "  width: 30px;\n"
        "  height: 30px;\n"
        "  border: 1px solid #90e0ef;\n"
        "  box-sizing: border-box;\n"
        "  position: relative;\n"
        "}\n"
        "#enemy-board .cell { border: 1px solid red; }\n"
        "#player-board .cell { border: 1px solid green; }\n"
        "\n"
        ".ship-cell::after {\n"
        "  content: \"\";\n"
        "  position: absolute;\n"
        "  inset: 3px;\n"
        "  background: #000;\n"
        "}\n"
        ".ship-single::after { border-radius: 999px; }\n"
        ".ship-h-mid::after { border-radius: 0; }\n"
        ".ship-h-start::after { border-top-left-radius: 999px; border-bottom-left-radius: 999px; border-top-right-radius: 0; border-bottom-right-radius: 0; }\n"
        ".ship-h-end::after { border-top-right-radius: 999px; border-bottom-right-radius: 999px; border-top-left-radius: 0; border-bottom-left-radius: 0; }\n"
        ".ship-v-mid::after { border-radius: 0; }\n"
        ".ship-v-start::after { border-top-left-radius: 999px; border-top-right-radius: 999px; border-bottom-left-radius: 0; border-bottom-right-radius: 0; }\n"
        ".ship-v-end::after { border-bottom-left-radius: 999px; border-bottom-right-radius: 999px; border-top-left-radius: 0; border-top-right-radius: 0; }\n"
    );
    fclose(f);

    f = fopen("script.js", "w");
    if (!f) { perror("script.js"); exit(1); }

    fprintf(f,
        "const letters = [\"A\",\"B\",\"C\",\"D\",\"E\",\"F\",\"G\",\"H\",\"I\",\"J\"];\n"
        "\n"
        "function buildBoard(boardId) {\n"
        "  const board = document.getElementById(boardId);\n"
        "  board.innerHTML = \"\";\n"
        "  board.innerHTML += `<div class=\"coord\"></div>`;\n"
        "  for (let i = 1; i <= 10; i++) board.innerHTML += `<div class=\"coord\">${i}</div>`;\n"
        "\n"
        "  for (let r = 0; r < 10; r++) {\n"
        "    board.innerHTML += `<div class=\"coord\">${letters[r]}</div>`;\n"
        "    for (let c = 0; c < 10; c++) {\n"
        "      board.innerHTML += `<div class=\"cell\" data-r=\"${r}\" data-c=\"${c}\"></div>`;\n"
        "    }\n"
        "  }\n"
        "}\n"
        "\n"
        "buildBoard(\"player-board\");\n"
        "buildBoard(\"enemy-board\");\n"
        "\n"
        "const playerShips = [\n"
    );

    for (int r = 0; r < N; r++) {
        fprintf(f, "  [");
        for (int c = 0; c < N; c++) {
            fprintf(f, "%d%s", board->cells[r][c], (c < N - 1) ? ", " : "");
        }
        fprintf(f, "]%s\n", (r < N - 1) ? "," : "");
    }

    fprintf(f,
        "];\n"
        "\n"
        "function classForShipCell(matrix, r, c) {\n"
        "  if (matrix[r][c] !== 1) return null;\n"
        "  const up = r > 0 && matrix[r-1][c] === 1;\n"
        "  const down = r < 9 && matrix[r+1][c] === 1;\n"
        "  const left = c > 0 && matrix[r][c-1] === 1;\n"
        "  const right = c < 9 && matrix[r][c+1] === 1;\n"
        "  const hasH = left || right;\n"
        "  const hasV = up || down;\n"
        "  if (!hasH && !hasV) return \"ship-cell ship-single\";\n"
        "  if (hasH) {\n"
        "    if (!left && right) return \"ship-cell ship-h-start\";\n"
        "    if (left && !right) return \"ship-cell ship-h-end\";\n"
        "    return \"ship-cell ship-h-mid\";\n"
        "  }\n"
        "  if (!up && down) return \"ship-cell ship-v-start\";\n"
        "  if (up && !down) return \"ship-cell ship-v-end\";\n"
        "  return \"ship-cell ship-v-mid\";\n"
        "}\n"
        "\n"
        "function drawShips(boardId, matrix) {\n"
        "  const board = document.getElementById(boardId);\n"
        "  const cells = board.querySelectorAll('.cell');\n"
        "  cells.forEach(cell => {\n"
        "    const r = parseInt(cell.dataset.r, 10);\n"
        "    const c = parseInt(cell.dataset.c, 10);\n"
        "    const cls = classForShipCell(matrix, r, c);\n"
        "    if (cls) cell.className = `cell ${cls}`;\n"
        "  });\n"
        "}\n"
        "drawShips('player-board', playerShips);\n"
    );

    fclose(f);
}
