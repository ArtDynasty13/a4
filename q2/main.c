#include "maze.h"
#include <stdio.h>
/*
 * This is the main that will use your main ADT.
 * It begins by calling your readMaze function to read
 * the maze in, and then continues by reading commands
 * from standard input from the user. The command
 * the user can give are as follows;
 *    p - to print the maze.
 *    q - to end the program.
 *    m - to make a move, the m must be followed by n, e, s, or w.
 *    r - to reset the maze by calling your reset function.
 * After reading all commands from the user your 
 * destroyMaze function is then called to clean up the maze.
*/

int whitespace(char c) {
  return c == ' ' || c == '\n' || c == '\n' || c == '\r';
}

char getNext() {
  char c = getchar();
  for (; whitespace(c); c = getchar());
  return c;
}

int main() {
  struct Maze *m = readMaze();
  if (m == NULL) {
    printf("Malformed maze.\n");
    return 0;
  }
  printf("Finished reading maze, enter commands\n");
  char cmd;
  char target;
  int done = 0;
  int win = 0;
  while (!done && !feof(stdin)) {
    cmd = getNext();
    if (cmd == EOF) break; // breaks loop
    switch (cmd) {
      case 'p':
        printMaze(m);
        break; // breaks switch
      case 'q':
        done = 1;
        break; // breaks switch
      case 'm':
        target = getNext();
        if (target == EOF) {
          done = 1;
          break; // breaks switch
        }
        if (target != 'n' && target != 'e' && target != 's' && target != 'w') {
          printf("Invalid move target: %c\n", target);
        } else {
          struct Pos p = makeMove(m, target);
          if (p.x == -1 && p.y == -1) {
            done = 1;
            win = 1;
          }
        }
        break;
      case 'r':
        reset(m);
        break;
      default:
        printf("Incorrect command: %c\n", cmd);
        break;
    }
  }
  if (win) {
    printf("Congrats you made it to the end of the maze!\n");
    printMaze(m);
  }
  m = destroyMaze(m);
  if (m != NULL) {
    printf("Error, your destroyMaze doesn't return NULL\n");
  }
}