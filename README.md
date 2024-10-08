# ConwaysGameOfLife
A terminal-based implimentation of Conway's Game of Life with player-defined rules.
# Installing
There currently isn't an official installer for the game. To play the game on linux, clone this repository, and in the directory compile the source code with the following command:<br><code>g++ main.cpp life.cpp -o <path/to/game/location></code><br>Additionally compile the game file creator from <code>src/utils/createFile.cpp</code> as this will allow you to generate starting states for the game.
# Game state files
This implementation of Conway's Game of Life reads the game's initial state from a provided file, these files can also be used to change the grid size of the game as well its rules. The first line of the file consists of 4 integers each seperated with a comma and space. The first two integers are the starting width and height of the game grid respectively. The second two are the maximum height and width of the game grid respectively. If the maximum height or width is set to 0, the grid will expand infinitely in that direction. The second line of the game state file defines the game's rules and consits of three integers seperated by semicolons. The first integer is the minimum number of neighbors a live cell must have to continue living, the second is the maximum number of neighbors that a cell may have in order to continue living, and the third is the number of neighbors a dead cell must have in order to come alive.<br>For example, the default rules of the game  (Any live cell with fewer than two live neighbours dies, as if by underpopulation. Any live cell with two or three live neighbours lives on to the next generation. Any live cell with more than three live neighbours dies, as if by overpopulation. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.) is represented as 2;3;3. 
## Creating a game state file
To create a game state file, run the binary compiled from <code>src/utils/createFile.cpp</code> with the following arguments: The starting width of the game, the starting height of the game, the maximum width of the game, the maximum height of the game and the name of the game state file to save.
## Editing a game state file
By default a game state file has no live cells in it, beneath the firsst two lines is text representing the game grid. Dead cells are represented as dots (.) and live cells are represented as O's (O).
# Playing the game
To play the game, run the comiled binary passing the game state file as an argument. <br>For example <code>./gameoflife testfile.txt</code>. The game asks for a simulation speed, this determines how many milliseconds each "generation" is displayed for. For example a simulation speed of 95 will reneder a new generation every 50 milliseconds, but a simulation speed of 1 will take 990 milliseconds to render each generation.
# Contributing
The main branch of this repository is not accepting direct code contributions at this time. You're encouraged to fork the repository and make your changes there. If you have a significant change that you believe should be incorporated into the main branch, feel free to discuss it by opening an issue.



