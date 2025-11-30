const letters = ["A","B","C","D","E","F","G","H","I","J"];

function buildBoard(boardId) {
const board = document.getElementById(boardId);

board.innerHTML += `<div class="coord"></div>`;

for (let i = 1; i <= 10; i++) {
  board.innerHTML += `<div class="coord">${i}</div>`;
}

for (let r = 0; r < 10; r++) {
  board.innerHTML += `<div class="coord">${letters[r]}</div>`;

  for (let c = 0; c < 10; c++) {
    board.innerHTML += `<div class="cell"></div>`;
  }
}
}

buildBoard("player-board");
buildBoard("enemy-board");
