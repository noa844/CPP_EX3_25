

#  Coup Game – C++ Implementation with SFML GUI

##  Project Overview

This project is an object-oriented C++ implementation of the strategic bluffing game **Coup**, with a custom SFML-based graphical interface. It uses several modern C++ features including `enum class`, dynamic casting, and `magic_enum` for clean and efficient enum manipulation.

The project supports:

* Random role assignment via a factory pattern.
* Core game logic including arrest, coup, sanction, tax, and bribe.
* Role-specific special abilities (e.g., Spy, Judge, Governor).
* Graphical interface powered by **SFML**, without external GUI dependencies.

---

##  Project Structure

```
.
├── game_logic/
│   ├── Game.hpp / .cpp           # Main Game class and core logic
│   ├── ActionType.hpp            # Game action types (Tax, Coup, etc.)
│   ├── DeletableActionType.hpp  # Actions that can be undone by roles
│   ├── Status.hpp               # Internal player flags
│   └── RoleType.hpp             # Enum for roles
│
├── player/
│   ├── Player.hpp / .cpp         # Base Player class
│   ├── PlayerFactory.hpp / .cpp # Generates player with random role
│   ├── SpecialActionUtils.hpp   # Metadata for role special abilities
│   ├── roles/
│   │   ├── Governor.hpp / .cpp
│   │   ├── Spy.hpp / .cpp
│   │   └── ...                   # All role classes
│
├── GUI/
│   ├── GameGui.hpp / .cpp        # GUI implementation using SFML
│
├── main.cpp                      # Entry point
├── Makefile                      # Build instructions
└── assets/
    └── background.png          # background used in GUI
```

---

##  Game Rules 

The implementation is based on an interpreted version of Coup’s gameplay and the assignment requirements. Here are some **important clarifications** and **custom decisions**:

*  **Game starts** only after clicking the `Start` button in the GUI.
*  If a player has **10 or more coins**, they **must perform a Coup** (not even allowed to bribe).
*  **Arrest cannot be used on the same player twice in a round** .
*  When arresting a **Merchant**, if they have fewer than 2 coins, they only lose **1 coin**.
*  A **revived player** returns to the same position in the turn order and but lose their coin count,
  and **their previous actions are erased**.
*  **Special actions** can technically be used outside a player's turn,
  but in the current GUI, some of them are only available during the player’s turn (for simplicity and clarity).

---

##  Why Use `magic_enum`?

This project relies heavily on [`magic_enum`](https://github.com/Neargye/magic_enum), a modern C++17 header-only library that enables reflection over `enum class`.

**Example Usage in This Project**:

* Automatically generating the list of `Status` or `RoleType` without manual updates.
* Populating maps like `statusMap` without boilerplate.
* Generating action names for debugging or GUI display.

### ✅ Benefits:

* Avoid manually maintaining lists of all enum values like `Status` or `RoleType`. This ensures consistency and reduces boilerplate.

---

##  How to Run the Project

###  Requirements

* C++17 compatible compiler (e.g. `g++`)
* SFML installed (`sudo apt install libsfml-dev`)
* Linux/macOS environment

###  Build & Run

1. **Compile and run GUI**:

```bash
make run
```

2. **(Optional)**: Run Gui with Valgrind to check memory usage:

```bash
make valgrind
```
3. **Compile and run Demo**: (also run with valgring)

```bash
make demo
```

---

##  GUI Instructions (SFML)

###  Start Screen

* Use your keyboard to **type the names** of players (up to 6).
* Press **Enter** after each name.
* Click **Start** to begin the game once all players are entered (minimum 2 players).

### 🕹️ Game Interface

* Displays:

  * All players' names, roles, and coin counts.
  * Who is currently playing.
  * Player status (Active / Eliminated).
* Shows:

  * Standard actions (Tax, Bribe, Coup, etc.) in the center.
  * Special abilities per role shown next to each player.
* To perform a special action or attack another player:

  * Select the corresponding button when it’s your turn.

### 🔁 Restart

Click **Restart** to reset everything and return to the name entry screen.

---

## Notes for Testing

* The UI displays **coin counts** to help with debugging.
* Some special action buttons are only enabled **during your turn**.
* Logs are printed in the console to trace every action.


---

##  Author
    Noa Honigstein
    noa.honigstein@gmail.com

