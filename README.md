# README.md

# AI-Based Chess Game (C++)

This repository is a minimal, **single-file-playable** chess engine + AI written in C++ with a command-line interface. It includes header files (`.h`) and implementation files (`.cpp`) and a `README` with build & run instructions.

**Features**
- Board representation (8x8)
- Basic legal move generation for all piece types (pawn, knight, bishop, rook, queen, king)
- Pawn promotion (to queen)
- No castling, no en-passant (kept simple for clarity)
- Simple position evaluation (material + piece-square tables)
- AI using Minimax with Alpha-Beta pruning
- CLI text-based board and move input (algebraic-like: e2e4)

**Files in this project**
- `types.h` — common enums and small structs
- `move.h` — Move struct and helpers
- `board.h` / `board.cpp` — Board class and move generation
- `ai.h` / `ai.cpp` — AI (minimax + alpha-beta)
- `game.h` / `game.cpp` — Game loop and CLI
- `main.cpp` — program entry

---

## Build

Compile with a modern C++ compiler supporting C++17. Example (Linux / macOS / WSL):

```bash
g++ -std=c++17 -O2 main.cpp board.cpp ai.cpp game.cpp -o chess
```

Or compile all in one command if you prefer single-file build (the headers are included):

```bash
g++ -std=c++17 -O2 main.cpp board.cpp ai.cpp game.cpp -o chess
```

## Run

```bash
./chess
```

**Controls**
- Enter moves in `e2e4` format (from-square to-square, lowercase files `a`-`h`, ranks `1`-`8`).
- To resign or quit type `quit` or `exit`.

## Tips
- Start with search depth 3 for quick responses. Depth 4 or 5 gives stronger play but is slower.
- This project is intentionally compact and readable; you can extend it by adding castling/en-passant, better evaluation, move ordering, iterative deepening, transposition table, or UCI support.

---
