#ifndef MAZE_H_
#define MAZE_H_
/*
 * DO NOT MODIFY THIS FILE.
 * ANY MODIFICATIONS TO THIS FILE WILL BE IGNORED EVEN IF
 * YOU SUBMIT THIS FILE, AS IT WILL BE REPLACED WITH THIS
 * ORIGINAL COPY. YOU ARE ONLY MEANT TO WRITE AND SUBMIT
 * THE IMPLEMENTATION FILE maze.c
 * YOU MAY ADD ANY ADDITIONAL HELPER FUNCTIONS AND TYPES YOU
 * NEED IN THAT IMPLEMENTATION FILE, BUT IT IS NOT NECESSARY
 * TO MODIFY THIS FILE
 */
struct Pos {
  int x, y;
};

// The Maze structure, free for you to define
// as you like in maze.c
struct Maze;

// The readMaze function, you must define it in maze.c
// The behaviour of this function is to read in a maze
// from standard input. Reads the maze line by line until 
// receiving an entirely empty line, at which point it 
// considers the maze complete.
// If it does not receive exactly one start tile and
// at least one goal tile it returns NULL
struct Maze *readMaze();

// The makeMove function, you must define it in maze.c
// The behaviour of the function is that it takes
// a pointer to a Maze and a character that is
// one of 'n', 'e', 's', or 'w' and moves the player
// in the given direction and returns to the
// caller the players new position. Unless the player
// has reached the goal position, then it returns
// the position (-1, -1).
struct Pos makeMove(struct Maze*, char);

// The reset function, you must define it in maze.c
// The behaviour of this function is that it takes
// a pointer to a Maze and resets the players position
// to the original starting position
void reset(struct Maze*);


// The printMaze function, you must define it in maze.c
// The behaviour of this function is that it prints
// the maze to the screen.
void printMaze(struct Maze*);

// Frees all memory associated with the given Maze
// which was allocated by the readMaze function
// including the Maze itself and returns NULL.
struct Maze *destroyMaze(struct Maze*);

#endif
