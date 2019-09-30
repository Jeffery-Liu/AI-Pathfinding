# AI-Pathfinding

# Prerequisites:
FW1FontWrapper.dll
32-bit FW1FontWrapper.dll was copied to the folder "Pathfinding".

If 32-bit "FW1FontWrapper.dll" doesn't work, then 64-bit "FW1FontWrapper.dll" can be found in the folder "Data".
Please find "FW1FontWrapper.dll 64bit.zip" from "Data" folder then extract the zip file.
Copy "FW1FontWrapper.dll" to the folder "Pathfinding" and replace the old "FW1FontWrapper.dll" file.

# Introduction:
The pathfinding project was developed based on the engine which developed by my college's professor Peter and me. 

The main purpose of this project is to find the path from the start point to the end point 
based on four different algorithms, and the algorithms are Breath First Serch (BFS), 
Depth First Search (DFS), Dijkstra (Dij) and A* (AStar).

The source files are located in the folder "Data/Source".

# Instruction:
The project will generate a window (1024 * 768) with (32 * 24) grids. 
Mouse:
Left mouse button: set up START point.
Right mouse button: set up END point.
Keyboard:
Hold "One": Add impassable obstacles.
Hold "Two": Add passable obstacles.
Hold "Three": Remove obstacles.

After setting up the START point, END poing and obstacles.
Keyboard:
Press "F1": Draw the path with Breath First Search algorithm.
Press "F2": Draw the path with Depth First Search algorithm.
Press "F3": Draw the path with Dijkstra algorithm.
Press "F4": Draw the path with A* algorithm.

## Contributors:
- Professor: Peter Chan
- Jinfeng (Jeffery) Liu <liujinfeng1209@gmail.com>

