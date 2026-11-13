# Advanced C: Can't Stop

Coursework from **Advanced C** at the **University of New Haven**, Spring 2025 (third year, spring semester).

- **Authors:** Ryan Godburn and Arjun Aravind (pair project)
- **Language:** C++17

Over ten modules, this project builds a complete console version of the board game **Can't Stop**, adding one class or feature per module. Each `P#` folder is a full snapshot of the project at the end of that module, with its own test plan and results document (`.docx`); most also include the test output.

## Modules

| Folder | Module focus |
| --- | --- |
| `P1-GodburnArjun` | `Dice` class: a dynamically allocated set of dice that roll and print. The unit test writes `diceTest.txt`. |
| `P2-GodburnAravind` | `player` class (name, color, score, columns won) and the color enums. |
| `P3-GodburnAravind` | `Column` class: placing a tower, moving it up, stopping to turn it into the player's tiles, and capturing a column. |
| `P4-GodburnAravind` | `Game` class: creates the dice and players from user input and drives tower placement and moves. |
| `P5-GodburnAravind` | `Board` class: all eleven columns, the active towers, and the move, stop and bust rules. |
| `P6-GodburnAravind` | `Game::oneTurn()`: the player chooses roll, stop or resign; after a roll they pair up the four dice (for example `AB`) and the matching towers move. |
| `P7-GodburnAravind` | `CList`: a circular linked list of players that controls turn order. Adds a UML class diagram. |
| `P8-GodburnAravind` | The full game loop, plus a dice hierarchy: `Dice` → `CSDice` → `FakeDice`, which reads scripted rolls from a file for repeatable testing. |
| `P9_GodburnAravind` | Exception handling: `BadPlayer` (`BadName`, `BadColor`) for duplicate players and `BadChoice` (`BadSlot`, `DuplicateSlot`) for invalid menu and dice input, with an output file for each case. |
| `P10-GodburnAravind` | Final version: choose real or fake dice, play with 2–4 players, build with the `Makefile`. Includes the full and summary UML diagrams and a dice-derivation diagram (PDF). |

`tools.hpp` / `tools.cpp` are the utility library provided for the course (banner, `fatal()` error handling, menus).

## Build and run

The final version has a Makefile (it uses `clang++`; change `CXX` to `g++` if you prefer):

```bash
cd P10-GodburnAravind
make
./CantStop
```

Any other module builds by compiling every `.cpp` in its folder:

```bash
cd P8-GodburnAravind
g++ -std=c++17 -o cantstop *.cpp
./cantstop
```

In P8–P10, choose "Fake dice" at the start to replay a scripted game from `fake_dice.txt` in the same folder.

P2 was developed on Windows, where file names aren't case-sensitive. To build it on Linux, change `#include "Dice.hpp"` in `dice.cpp` to `#include "dice.hpp"`.

---

*Archived coursework, shared as part of my portfolio. If you're taking this course now, please follow your instructor's academic integrity policy.*
