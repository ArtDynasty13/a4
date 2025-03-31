#include <stdio.h>
#include <stdlib.h>
#include "maze.h" //do we need this for certain?

struct Maze {
    char **tiles;
    int row_count;
    int top;
    int bottom;
    struct Pos player;
    struct Pos start;
};

int isDigit(char c) {
    if(c >= '0' && c <= '9') return 1;
    return 0;
}

//returns NULL immediately after a newline is received and more than 1 S's are read in
//returns NULL if no goal is read in
//iterate until an "entirely empty line"

struct Maze *readMaze() {
    //int row_count = 0;
    int row_cap = 4;
    char c;
    struct Maze *maze = malloc(sizeof(struct Maze));
    maze->tiles = malloc(sizeof(char *)*row_cap);
    maze->row_count = 0;

    int start_count = 0;
    int goal_count = 0;
    
    // Skip all leading whitespace (space, \t, \n, \r, \v, \f)
    while ((c = getchar()) != EOF) {
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r' && c != '\v' && c != '\f') {
            ungetc(c, stdin);  // Put back the non-whitespace character
            break;
        }
    }
    
    // If we hit EOF before any maze content
    if (c == EOF) {
        free(maze->tiles);
        free(maze);
        return NULL;
    }

    while(1) {

        if((c = getchar()) == '\n') { //we receive two newlines in a row
            if(start_count == 1 && goal_count >= 1) {
                return maze;
            } else {
                destroyMaze(maze);
                return NULL;
            }
        }

        if (maze->row_count == row_cap) {
            char **new_rows = malloc(sizeof(char *)*row_cap*2);
            row_cap *= 2;
            for(int i = 0; i < maze->row_count; ++i) {
                new_rows[i] = maze->tiles[i];
            }
            free(maze->tiles);
            maze->tiles = new_rows;
        }

        int column_count = 0;
        int column_cap = 4;
        maze->tiles[maze->row_count] = malloc(sizeof(char)*column_cap +1); //add 1 for the null terminator
        for(;c != '\n' && c != EOF; c = getchar()) {
            if(column_cap == column_count) {
                char *new_column = malloc(sizeof(char)*(column_cap*2+1));
                column_cap *= 2;
                for(int i = 0; i < column_count; ++i) {
                    new_column[i] = maze->tiles[maze->row_count][i];
                }
                free(maze->tiles[maze->row_count]);
                maze->tiles[maze->row_count] = new_column;
            }
            maze->tiles[maze->row_count][column_count++] = c;
            if(maze->row_count == 0) {
                maze->top = column_count;
            }
            if(c == 'S')  {
                ++start_count;
                struct Pos start = {column_count-1, maze->row_count};
                maze->start = start;
                maze->player = start;
                if(start_count != 1) {
                    maze->row_count++;
                    destroyMaze(maze);
                    return NULL;
                }
            } else if (c == 'G') {
                ++goal_count;
            }
        }
        maze->tiles[maze->row_count][column_count] = '\0';
        ++maze->row_count;
        maze->bottom = column_count;
    }
}

void printMaze(struct Maze *maze) {
    for(int i = 0; i < maze->top+2; ++i) printf("-");
    printf("\n");
    for(int i = 0; i < maze->row_count; ++i) {
        printf("|");
        for(int k = 0; maze->tiles[i][k] != '\0'; k++) {
            if(k == maze->player.x && i == maze->player.y) {
                printf("%c", 'P');
            }else {
                printf("%c", maze->tiles[i][k]);
            }
        }
        printf("|");
        printf("\n");
    }
    for(int i = 0; i < maze->bottom+2; ++i) printf("-");
    printf("\n");
}

struct Pos makeTeleport(struct Maze* maze, char n, struct Pos cur_t_pos) {
    for(int i = 0; i < maze->row_count; ++i) {
        for(int k = 0; maze->tiles[i][k] != '\0'; k++) {
            if(maze->tiles[i][k] == n) {
                if(cur_t_pos.y == i && cur_t_pos.x == k) {
                    continue;
                }
                struct Pos new_pos = {k, i};
                return new_pos;
            }
        }
    }
    return maze->player;
}

// The makeMove function, you must define it in maze.c
// The behaviour of the function is that it takes
// a pointer to a Maze and a character that is
// one of 'n', 'e', 's', or 'w' and moves the player
// in the given direction and returns to the
// caller the players new position. Unless the player
// has reached the goal position, then it returns
// the position (-1, -1).
struct Pos makeMove(struct Maze* maze, char c) {
    int dx = 0, dy = 0;
    switch(c) {
        case 'n': dy = -1; break;
        case 's': dy = 1; break;
        case 'e': dx = 1; break;
        case 'w': dx = -1; break;   
    }
    int new_x = maze->player.x + dx;
    int new_y = maze->player.y + dy;

    if(new_x < 0 || maze->tiles[maze->player.y][new_x] == '\0' || new_y < 0 || new_y >= maze->row_count) { //check bounds
        //fprintf(stderr, "out of bounds");
        return maze->player;
    }

    char tile = maze->tiles[new_y][new_x];

    if(tile == 'X') { //wall
        return maze->player;
    } else if (tile == 'O' || tile == 'S') { //open OR START IS ALSO LEGAL
        struct Pos new_pos = {new_x,new_y};
        maze->player = new_pos;
        return new_pos;
    } else if (tile == 'I') {
        struct Pos new_pos = {new_x,new_y};
        maze->player = new_pos;
        return makeMove(maze, c); //recursive call if its an ICY tile, does this work?
    } else if (tile == 'G') {
        struct Pos return_pos = {-1, -1};
        struct Pos goal_pos = {new_x,new_y};
        maze->player = goal_pos;
        return return_pos;
    } else if (isDigit(tile)) {
        struct Pos cur_t_pos = {new_x, new_y};
        maze->player = makeTeleport(maze, tile, cur_t_pos);
        return makeTeleport(maze, tile, cur_t_pos);
    } else {
        return maze->player;
    }
}

void reset(struct Maze* maze) {
    maze->player = maze->start;
}

// Frees all memory associated with the given Maze
// which was allocated by the readMaze function
// including the Maze itself and returns NULL.
struct Maze *destroyMaze(struct Maze* maze) {
    for(int i = 0; i < maze->row_count; ++i) {
        free(maze->tiles[i]);
    }
    free(maze->tiles);
    free(maze);
    return NULL;
}