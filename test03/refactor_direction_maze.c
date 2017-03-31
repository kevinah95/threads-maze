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

typedef struct pointer_struct
{
    char *character;
    int direction_old;
    int direction_new;
    int x_pos;
    int y_pos;
    int color;
} *pointer_t ;


/* Display the maze. */
void show_maze(const char *maze, int width, int height) {
    int x, y;
    //clear();
    for (y = 0; y < height; y++) {
        //int rand_num =  rand_interval(0,7);
        for (x = 0; x < width; x++) {
            //gotoxy(x,y);
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
        printf("\n");
    }
}

void move(char *maze, int width, int height, pointer_t pointer) {


    //-------------
    int left = (pointer->x_pos > 0) ? (pointer->x_pos - 1) : pointer->x_pos;
    int right = (pointer->x_pos < width - 1) ? (pointer->x_pos + 1) : pointer->x_pos;
    int up = (pointer->y_pos > 0) ? (pointer->y_pos - 1) : pointer->y_pos;
    int down = (pointer->y_pos < height - 1) ? (pointer->y_pos + 1) : pointer->y_pos;
    //-------------


    //--------------for
    if (maze[pointer->x_pos + width * pointer->y_pos] == ' ') {
        maze[pointer->x_pos + width * pointer->y_pos] = pointer->character;
        /*gotoxy(pointer->x_pos,pointer->y_pos);
        PRINTC (pointer->color, "%c", pointer->character);*/
    } else {
        return;
    }

    if(pointer->direction_old != pointer->direction_new){
        printf("NEW_DIR %d X:%d Y:%d OLD:%d",pointer->direction_new,pointer->x_pos,pointer->y_pos,pointer->direction_old);
        int rand_num =  rand_interval(0,7);
        char randomletter = 'a' + (random() % 26);
        pointer->color = rand_num;
        pointer->character = randomletter;
        pointer->direction_old = pointer->direction_new;
    }

    char left_flag = maze[left + width * pointer->y_pos];
    if (left_flag == ' ') {
        pointer->x_pos = left;
        printf("\nCREATE_LEFT:%d %d", pointer->x_pos, pointer->y_pos);
        pointer->direction_old = pointer->direction_new;
        pointer->direction_new = WEST;
        move(maze, width, height,pointer);
    }
    char right_flag = maze[right + width * pointer->y_pos];
    if (right_flag == ' ') {
        pointer->x_pos = right;
        printf("\nCREATE_RIGHT:%d %d", pointer->x_pos, pointer->y_pos);
        pointer->direction_old = pointer->direction_new;
        pointer->direction_new = EAST;
        move(maze, width, height, pointer);
    }
    char up_flag = maze[pointer->x_pos + width * up];
    if (up_flag == ' ') {
        pointer->y_pos = up;
        printf("\nCREATE_UP:%d %d", pointer->x_pos, pointer->y_pos);
        pointer->direction_old = pointer->direction_new;
        pointer->direction_new = NORTH;
        move(maze, width, height, pointer);
    }

    char down_flag = maze[pointer->x_pos + width * down];
    if (down_flag == ' ') {
        pointer->y_pos = down;
        printf("\n---CREATE_DOWN:%d %d", pointer->x_pos, pointer->y_pos);
        pointer->direction_old = pointer->direction_new;
        pointer->direction_new = SOUTH;
        move(maze, width, height, pointer);
    }
    move(maze, width, height, pointer);

    return;
}

void gotoxy(int x,int y){
    x+=1;
    y+=1;
    printf("%c[%d;%df",0x1B,y,x);
}



int main(int argc, char *argv[]) {
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
    show_maze(bytes, 8, 8);//Don't Remove

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

    move(bytes, 8, 8,pointer);
    //move_right(bytes, 8, 8, 0, 7);
    //move_up(bytes, 8, 8, 4, 8); //(x_pos - 1) (y_pos - 1)
    //move_left(bytes, 8, 8, 2, 7); //(x_pos - 1) (y_pos - 1)
    printf("\n");
    show_maze(bytes, 8, 8);
    printf("\n\n"); //Don't Remove
    free(bytes);
    return 0;
}
