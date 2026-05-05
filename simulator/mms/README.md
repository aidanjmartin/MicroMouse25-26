# mms — Micromouse Simulator

This project uses [`mms`](https://github.com/mackorone/mms) by **Mack Ward** ([@mackorone](https://github.com/mackorone)) to test maze-solving code without a physical robot. The simulator is **not vendored** here — install it from upstream:

- **Source / releases:** https://github.com/mackorone/mms
- **Docs / API reference:** https://github.com/mackorone/mms#readme

## How our solver plugs in

Our maze-solving algorithm lives in [`../SimulationProgram/`](../SimulationProgram). It targets the `mms` Mouse API:

1. Build the solver in `simulator/SimulationProgram/` (produces an executable).
2. In `mms`, add a new mouse and point its **Build Command** / **Run Command** at that executable.
3. Run the simulation.

## Credits & acknowledgements

`mms` is the work of [@mackorone](https://github.com/mackorone) — all credit for the simulator goes to him. See the upstream repo for license terms.

Our adoption of `mms` was guided by the **IEEE at UCLA Micromouse** program's public materials:

- Project docs: https://projects.ieeebruins.com/micromouse
- UCLA's own (separate, C/SDL2-based) simulator: https://github.com/Tyler4p/Micromouse-Maze-Simulator

UCLA uses but did not author `mms`; their tutorials were a useful reference.
