#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>


#define MILLISECOND 1000
#define SECOND 100*MILLISECOND //1000*MILLISECOND <- One Second

#define PRINTC(c, f, s0) printf ("\033[%dm" f "\033[0m", 30 + c, s0)
#define clear() printf("\033[H\033[J")

enum directionFlags {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
};

typedef struct pointer_struct
{
    char *character;
    int direction_old;
    int direction_new;
    int x_pos;
    int y_pos;
    int color;
} *pointer_t ;




unsigned int rand_interval(unsigned int min, unsigned int max) {
    int r;
    const unsigned int range = 1 + max - min;
    const unsigned int buckets = RAND_MAX / range;
    const unsigned int limit = buckets * range;

    /* Create equal size buckets all in a row, then fire randomly towards
     * the buckets until you land in one of them. All buckets are equally
     * likely. If you land off the end of the line of buckets, try again. */
    do {
        r = rand();
    } while (r >= limit);

    return min + (r / buckets);
}



/* Display the maze. */
void show_maze(const char *maze, int width, int height) {
    int x, y;
    clear();
    for (y = 0; y < height; y++) {
        //int rand_num =  rand_interval(0,7);
        for (x = 0; x < width; x++) {
            gotoxy(x,y);
            printf("%c", maze[x + width * y]);
            /*fflush(stdout);
            usleep(SECOND);*/
            /*switch(maze[y * width + x]) {
                case '#':  printf("[]");  break;
                case ' ':  printf("  ");  break;
                //default: printf("%c%c", maze[x + width * y],maze[x + width * y]);  break;
                default: PRINTC (rand_num, "%c%c", maze[x + width * y],maze[x + width * y]);  break;
            }*/
        }
    }
}

void move(char *maze, int width, int height, pointer_t pointer) {
    int x = pointer->x_pos;
    int y = pointer->y_pos;
    //-------------
    int left = (pointer->x_pos > 0) ? (pointer->x_pos - 1) : pointer->x_pos;
    int right = (pointer->x_pos < width - 1) ? (pointer->x_pos + 1) : pointer->x_pos;
    int up = (pointer->y_pos > 0) ? (pointer->y_pos - 1) : pointer->y_pos;
    int down = (pointer->y_pos < height - 1) ? (pointer->y_pos + 1) : pointer->y_pos;

    if(pointer->direction_old != pointer->direction_new){
        int rand_num =  rand_interval(0,7);
        char randomletter = 'a' + (random() % 26);
        pointer->color = rand_num;
        pointer->character = randomletter;
    }
    //--------for
    if (maze[x + width * y] == ' ') {
        maze[x + width * y] = pointer->character;
        gotoxy(x,y);
        PRINTC (pointer->color, "%c", maze[x + width * y]);
    } else {
        return;
    }


    char left_flag = maze[left + width * y];
    if (left_flag == ' ') {
        //printf(" CREATE_LEFT:%d %d", x, y);
        pointer->x_pos = left;
        pointer->y_pos = y;
        pointer->direction_old = pointer->direction_new;
        pointer->direction_new = WEST;
        move(maze, width, height, pointer);
    }

    char right_flag = maze[right + width * y];
    if (right_flag == ' ') {
        //printf(" CREATE_RIGHT:%d %d", x, y);
        pointer->x_pos = right;
        pointer->y_pos = y;
        pointer->direction_old = pointer->direction_new;
        pointer->direction_new = EAST;
        move(maze, width, height, pointer);
    }
    char up_flag = maze[x + width * up];
    if (up_flag == ' ') {
        //printf(" CREATE_UP:%d %d", x, y);
        pointer->x_pos = x;
        pointer->y_pos = up;
        pointer->direction_old = pointer->direction_new;
        pointer->direction_new = NORTH;
        move(maze, width, height, pointer);
    }

    char down_flag = maze[x + width * down];
    if (down_flag == ' ') {
        //printf(" CREATE_DOWN:%d %d", x, y);
        pointer->x_pos = x;
        pointer->y_pos = down;
        pointer->direction_old = pointer->direction_new;
        pointer->direction_new = SOUTH;
        move(maze, width, height, pointer);
    }
}




void gotoxy(int x,int y){
    x+=1;
    y+=1;
    printf("%c[%d;%df",0x1B,y,x);
}

int main(void) {
    /* Intializes random number generator */
    srand(time(0));


    FILE *f = fopen("examples/maze.txt", "rb");
    fseek(f, 0, SEEK_END);
    long pos = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *bytes = malloc((pos) * sizeof(char *));
    fscanf(f, "%[^\\n\\r]", bytes);

    fclose(f);
    char *temp;
    // Remove \n.
    while ((temp = strstr(bytes, "\n")) != NULL) {
        // Len is the length of the string, from the ampersand \n, including the \n.
        int len = strlen(bytes);
        memcpy(temp, temp + 1, len);
    }
    show_maze(bytes, 8, 8);


    pointer_t pointer;
    char randomletter = 'a' + (random() % 26);
    int rand_num =  rand_interval(0,7);
    pointer = (pointer_t *)malloc(1 * sizeof(pointer_t ));
    pointer->character = randomletter;
    pointer->direction_old = SOUTH;
    pointer->direction_new = SOUTH;
    pointer->x_pos = 0;
    pointer->y_pos = 0;
    pointer->color = rand_num;

    move(bytes, 8, 8, pointer);
    //move_right(bytes, 8, 8, 0, 7);
    //move_up(bytes, 8, 8, 4, 8); //(x_pos - 1) (y_pos - 1)
    //move_left(bytes, 8, 8, 2, 7); //(x_pos - 1) (y_pos - 1)
    //---printf("\n");
    //show_maze(bytes, 8, 8);
    printf("\n\n"); //Don't Remove
    free(bytes);
    return 0;
}
