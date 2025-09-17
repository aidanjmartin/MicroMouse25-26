# Micromouse Maze Solver

This is a C-based Micromouse simulator project inspired by UCLA IEEE Micromouse resources. The robot navigates a virtual maze using a flood-fill algorithm and updates its understanding of the environment in real-time.

## 📁 Project Structure


├── main.c          # Runs the control loop and calls solver()
├── solver.c        # Contains the maze-solving logic (flood-fill and wall memory)
├── solver.h        # Solver function declarations
├── API.c / API.h   # Provided by the simulator (interface to the virtual maze)
├── README.md       # Project description and usage


## How It Works

- The micromouse starts at position (0, 0), facing **north**.
- It uses **IR sensors** (simulated) to detect walls in front, left, and right.
- The mouse keeps a local memory of walls and updates it as it explores.
- A **flood-fill algorithm** calculates the distance from each cell to the center.
- The mouse always moves toward the lowest neighbor cell in the flood map.

## Algorithm Features

- Wall detection and memory with symmetry
- Dynamic flood-fill updates using BFS
- U-turn handling for dead ends
- Center goal targeting (cells [7,7], [7,8], [8,7], [8,8])
- Flood map visualization for debugging (`API_setText()`)

## 🔧 Requirements

- A C compiler (e.g., `gcc`, `clang`)
- Micromouse simulator supporting UCLA-style `API.h` (e.g., [UCLA IEEE Simulator](https://projects.ieeebruins.com/micromouse/))

## ▶️ How to Run

1. Download or clone the project.
2. Build the code using your simulator’s instructions.
3. Start the simulator and run:


./micromouse


4. Watch the bot solve the maze. Flood values will be shown on the grid.

## Team & Contributions

This project is part of a team Micromouse build. Special thanks to:

- Aidan Martin 
- Dalyn Wachala
- Gabrielle Miller

Based on resources from: [UCLA IEEE Micromouse](https://projects.ieeebruins.com/micromouse/)

## TODO

- Add fast-run after mapping
- Goal detection and reverse planning
- Optimize wall memory layout
- IR sensor calibration for physical mouse
- 3D-printed chassis integration
