function createBoard(boardId) {
  const board = document.getElementById(boardId);

  for (let i = 0; i < 100; i++) {
    const cell = document.createElement("div");
    cell.classList.add("cell");
    board.appendChild(cell);
  }
}

createBoard("board-player");
createBoard("board-enemy");


