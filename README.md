# Earth vs. Alien Invasion — Battle Simulation
 
A C++ console application (Visual Studio solution `ConsoleApplication1`) that simulates a
turn-based war between an **Earth Army**, an **Allied Army**, and an **Alien Army**. Units are
generated randomly each time step according to config-file probabilities, armies attack each
other automatically based on a fixed combat priority order, and the simulation runs until one
side is wiped out (or a maximum time step is reached). Results are written to a detailed
statistics report file.
 
## Overview
 
Each time step (`timeStep`) the game:
 
1. Randomly generates new **Earth units** (Soldiers, Tanks, Gunnery, Heal units) and **Alien
   units** (Soldiers, Monsters, Drones) based on configured spawn probabilities.
2. Optionally generates **Allied Support Units (SU)** when Earth's infection level crosses a
   threshold (`needSU()`).
3. Resolves combat for the step (`AttackLogic()`) — each unit type attacks specific enemy types
   in a defined priority order (see below).
4. Spreads infection among Earth Soldiers (`infectionspread()`).
5. Repeats until `input == -1`, one army is destroyed, or `timeStep` exceeds 40 (with a "no more
   valid attacks" flag as an early-exit condition).
At the end, `CreateOutputFile()` writes a full battle report (per-unit death timeline plus
aggregate statistics) to `<inputFileName>.txt`.
 
## Unit Types
 
| Side   | Unit           | Type ID | Storage structure                          |
|--------|----------------|:-------:|---------------------------------------------|
| Earth  | Earth Soldier  | 0       | Singly linked queue                          |
| Earth  | Earth Tank     | 1       | Stack                                        |
| Earth  | Gunnery        | 2       | Priority queue (priority = health × power)   |
| Earth  | HEAL unit      | 6       | Stack                                        |
| Alien  | Alien Soldier  | 3       | Singly linked queue                          |
| Alien  | Alien Monster  | 4       | Fixed-size array (random-access removal)     |
| Alien  | Alien Drone    | 5       | Custom doubly linked queue (front & back ops)|
| Allied | Support Unit (SU) | 7   | Singly linked queue                          |
 
All unit types derive from a common `Unit` base class (health, power, attack cap, infection /
immunity flags, join time `Tj`, first-attacked time `Ta`, death time `Td`).
 
## Combat Priority
 
`Game::AttackLogic()` resolves one attack round per side, per step, in this order:
 
- **Allied SU** → Alien Soldiers
- **Earth Soldiers** → Alien Soldiers (or, if infected, attack their *own* side instead)
- **Earth Tanks** → Alien Monsters, then Alien Soldiers (soldier ratio dependent)
- **Earth Gunnery** → Alien Monsters, then Alien Drones (front & back of queue)
- **Alien Soldiers** → Allied SU, then Earth Soldiers
- **Alien Monsters** → Allied SU, Earth Soldiers, Earth Tanks
- **Alien Drones** → Earth Gunnery, Earth Tanks
Units that survive an attack are re-queued to their army; units that reach ≤ 0 health are moved
to the `KilledList` with a recorded death time. Heavily wounded Earth Soldiers/Tanks (health <
20% of initial) are moved to an "Under Medical List" (UML) priority queue for possible healing.
 
## Infection Mechanic
 
- Alien Monsters can infect Earth Soldiers on attack, based on `infectionprob`.
- Infected soldiers have a chance each step to infect a random fellow soldier
  (`infectionspread()`), unless that soldier is immune.
- `HEAL` units cure infected units (granting immunity) or restore health to non-infected units,
  scaled by the healer's stats and the target's current health.
- Allied Support Units are only generated once the infection rate among Earth Soldiers exceeds
  a configured `infectionthreshold`.
## Data Structures
 
Custom, hand-implemented container classes are used throughout (no STL containers for game
state):
 
- `LinkedQueue<T>` — singly linked FIFO queue (also used as a priority queue variant with a
  numeric priority key)
- `DoubleLinkedQueue<T>` (`DoubleLinkedQueue.h`) — doubly linked queue supporting
  enqueue/dequeue from **both** ends (`enqueuefirst`, `dequeuelast`, `peeklast`) — used for
  Alien Drones
- A custom `Stack<T>` — used for Earth Tanks and HEAL units
- A fixed-size array — used for Alien Monsters (order doesn't matter, so O(1) insert/remove via
  swap-with-last)
## Input File Format
 
At startup the program asks for a file name, then loads `<name>.txt` containing space-separated
simulation parameters (spawn counts/percentages per unit type, health/power/attack-cap ranges
for Earth/Alien/SU units, infection probability, and infection threshold). See the codebase
constants (`N`, `ES`, `ET`, `EG`, `HU`, `AS`, `AM`, `AD`, `Prob`, etc.) in `Game.h` for the exact
field order expected.
 
> Note: `Weak_Earth_Vs_Moderate_Alien.txt` included in this repo is a **sample output report**
> from a prior run (named after that scenario), not an input config file.
 
## Output
 
Running the game writes `<inputFileName>.txt`, containing:
 
- A chronological list of every destroyed unit (`Td`, `ID`, `Tj`, `Df`, `Dd`, `Db` — death time,
  ID, join time, time-to-first-attack, time-to-death-after-first-attack, total time alive)
- Per-army totals and destruction percentages (Soldiers, Tanks, Gunnery / Soldiers, Monsters,
  Drones)
- Battle result (Win / Loss / Drawn)
- Average survival metrics and healing/infection statistics
## Modes
 
On launch you're prompted to choose:
 
1. **Interactive Mode** — pauses/prints detailed per-round attack info
2. **Silent Mode** — runs to completion with minimal console output
## Building
 
Open `ConsoleApplication1.sln` in Visual Studio 2022 (or newer) and build. Configurations
available: `Debug|x86`, `Debug|x64`, `Release|x86`, `Release|x64`.
 
## Project Structure
 
```
main.cpp              Entry point — constructs Game
Game.cpp/.h            Core simulation loop, combat logic, file I/O, statistics
EarthArmy.cpp          Container/management class for all Earth units
AlienArmy.cpp          Container/management class for all Alien units
AlliedArmy.cpp         Container/management class for Allied Support Units
EarthSoldier/Tank/Gunnery/HEAL.cpp   Earth unit subclasses (Unit)
AlienSoldier/Monster/Drone.cpp        Alien unit subclasses (Unit)
randGen.cpp             Random unit/number generation utilities
DoubleLinkedQueue.h      Doubly linked queue ADT (used for Alien Drones)
```
 
## Requirements
 
- Windows + Visual Studio 2022 (v143 toolset implied by `.sln`), or any C++ compiler with
  Visual Studio project support
- C++11 or later
