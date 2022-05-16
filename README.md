# Crimson Rush
This project was created with the intention of learning scripting with a defined engine built in C++. Pairing this with ECS (Entity Component System) and exposing it to a scripting language like Lua the more game related part of the application can be built using already defined functions from C++.

The rendering side of the application uses [Raylib](https://github.com/raysan5/raylib), the ECS is defined using  [EnTT](https://github.com/skypjack/entt) and bridging lua to C++ is done with the [Lua source code](https://www.lua.org/home.html).

## Features
List of functions that are exposed to and used in the lua scripts
* Entity creation and removal
* Adding/removing/alteration to Components. List of components:
	* Transform
	* Mesh
	* UI Element (text, colour, positions, size etc.)
	* Behaviour (init, update, collision. Activated on C++ side of engine)
* Creation of Systems.
* Switching scenes
* Loading models and using strings to signify asset used (Resource manager)
* Getting input (keyboard, mouse)

## Game implementation
Crimson Rush is a *dungeon-crawler* and *rougelike* inspired game all about exploring rooms and surviving as long as possible. The game is in *top-down-view* and the player has a pistol used to kill enemies encountered in the different rooms. When entering a room, the doors locks until the player has defeated all enemies. The player is damaged when coming in contact with an enemy

The map that is being explored by the player has a start and exit room. When the player reaches the exit room and exits, the map is recreated and a new level has been reached. The player also get a little health boost.
This is repeated until the player is dead and after a screen showing the level reached is shown.

![Room Example](../assets/Enemy.png)
 
### Room creator
The game also comes with a room creator where the width, height and placement for doors and enemies can be decided. After deciding width and height a grid of tiles are created. The edges of the room are defined as walls and the rest floor. Left clicking the tiles changes the state of it. Floor tiles can change to enemy tiles and wall tiles can change to doors and vice versa. The colour of the tile represent the state of it.

**Colours:**
* ⬜ : Floor
* 🟥 : Enemy
* 🟦 : Wall
* 🟨 : Door

![Room Creator](../assets/Room.png)

After saving a room, the contents is stored using self defined format called *.room*. This file format use different characters to define the states of the tiles in the room.

**Example:**

    ----+----
    ---------
    ---------
    -----*---
    +-------+
    ---*-----
    ---------
    ---------
    ----+----
  The *plus symbol* defines doors and the *multiplication symbol* enemies. The rest using the *minus symbol* is either floor or walls. When the game imports the rooms the position of the symbols in the *.room* file decides which tiles they affect.

## Controls
* WASD - moving the player
* Mouse - rotating the player around the y-axis. Used for aiming the pistol
* Leftclick (Mouse) - shoot bullet
* P - pause/resume game

## Building
Building the game is done using Visual Studio 2022 with the solution file provided. After building the application it can either be started using the "Local Windows Debugger" in visual studio or by starting the .exe file in the *bin* folder created. 

*(NOTE: If using the .exe file, make sure the folders *Scripts/* and *Resources/* also are added to the same folder. They are stored in the *Application* folder)*
