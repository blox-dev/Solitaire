# 🎮 Solitaire

An implementation of **Peg Solitaire** (also known as **Solo Noble**, **Marble Solitaire**, or simply **Solitaire**) in C++ using the SDL2 library. This version of the game features a board where pegs are moved to empty holes with the objective of leaving just one peg in the center.

## 📜 Game Rules

The objective of the game is to start with a full board, leaving only one peg in the central hole after making valid moves.

- The board is initially filled with pegs, except for the central hole.
- The goal is to remove pegs by jumping over them into empty spaces.
- A move is valid if a peg jumps over an adjacent peg into an empty hole.
- The game ends when only one peg remains in the central hole.

## 🚀 Getting Started

Follow the instructions below to get this project up and running on your local machine.

### 🛠️ Prerequisites

Before building the project, ensure that you have the following installed:

- **CMake v3.10+**: [CMake website](https://cmake.org/)
- **C++ Compiler**: The compiler should support at least the **C++17** standard. Supported compilers include *MSVC*, *GCC*, *Clang*.

### 🏗️ Building the Project

To build the project, follow these steps:

```bash
mkdir build/ && cd build/
cmake ..
cmake --build .
```

This will compile the project and generate the executable. You can then run the game on your local machine.
