const letters = ["A","B","C","D","E","F","G","H","I","J"];

function buildBoard(boardId) {
  const board = document.getElementById(boardId);
  board.innerHTML = "";
  board.innerHTML += `<div class="coord"></div>`;
  for (let i = 1; i <= 10; i++) board.innerHTML += `<div class="coord">${i}</div>`;

  for (let r = 0; r < 10; r++) {
    board.innerHTML += `<div class="coord">${letters[r]}</div>`;
    for (let c = 0; c < 10; c++) {
      board.innerHTML += `<div class="cell" data-r="${r}" data-c="${c}"></div>`;
    }
  }
}

buildBoard("player-board");
buildBoard("enemy-board");

const playerShips = [
  [1, 1, 0, 0, 1, 0, 0, 1, 0, 0],
  [0, 0, 0, 0, 1, 0, 0, 1, 0, 0],
  [1, 1, 1, 0, 1, 0, 0, 1, 0, 0],
  [0, 0, 0, 0, 1, 0, 0, 1, 0, 0],
  [0, 0, 0, 0, 0, 0, 0, 1, 0, 0],
  [1, 1, 1, 0, 0, 0, 0, 0, 0, 0],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
];

function classForShipCell(matrix, r, c) {
  if (matrix[r][c] !== 1) return null;
  const up = r > 0 && matrix[r-1][c] === 1;
  const down = r < 9 && matrix[r+1][c] === 1;
  const left = c > 0 && matrix[r][c-1] === 1;
  const right = c < 9 && matrix[r][c+1] === 1;
  const hasH = left || right;
  const hasV = up || down;
  if (!hasH && !hasV) return "ship-cell ship-single";
  if (hasH) {
    if (!left && right) return "ship-cell ship-h-start";
    if (left && !right) return "ship-cell ship-h-end";
    return "ship-cell ship-h-mid";
  }
  if (!up && down) return "ship-cell ship-v-start";
  if (up && !down) return "ship-cell ship-v-end";
  return "ship-cell ship-v-mid";
}

function drawShips(boardId, matrix) {
  const board = document.getElementById(boardId);
  const cells = board.querySelectorAll('.cell');
  cells.forEach(cell => {
    const r = parseInt(cell.dataset.r, 10);
    const c = parseInt(cell.dataset.c, 10);
    const cls = classForShipCell(matrix, r, c);
    if (cls) cell.className = `cell ${cls}`;
  });
}
drawShips('player-board', playerShips);
