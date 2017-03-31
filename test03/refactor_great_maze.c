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



struct thread_args {
    char *maze;
    int width;
    int height;
    int x_pos;
    int y_pos;
};

void move_down(struct thread_args **argp) {
    struct thread_args *args = *argp;
    int x = args->x_pos;
    int y = args->y_pos;
    char randomletter = 'a' + (random() % 26);
    //-------------
    int left = (args->x_pos > 0) ? (args->x_pos - 1) : args->x_pos;
    int right = (args->x_pos < args->width - 1) ? (args->x_pos + 1) : args->x_pos;
    //-------------
    int rand_num =  rand_interval(0,7);
    for (y; y < args->height; y++) {
        if (args->maze[x + args->width * y] == ' ') {
            args->maze[x + args->width * y] = randomletter;
            gotoxy(x,y);
            PRINTC (rand_num, "%c", args->maze[x + args->width * y]);
        } else {
            return;
        }
        char left_flag = args->maze[left + args->width * y];
        if (left_flag == ' ') {
            //printf(" CREATE_LEFT:%d %d", x, y);
            args->x_pos = left;
            args->y_pos = y;
            move_left(&args);
        }

        char right_flag = args->maze[right + args->width * y];
        if (right_flag == ' ') {
            //printf(" CREATE_RIGHT:%d %d", x, y);
            args->x_pos = right;
            args->y_pos = y;
            move_right(&args);
        }
    }
}

void move_right(struct thread_args **argp) {
    struct thread_args *args = *argp;
    int x = args->x_pos;
    int y = args->y_pos;
    char randomletter = 'a' + (random() % 26);
    //-------------
    int up = (args->y_pos > 0) ? (args->y_pos - 1) : args->y_pos;
    int down = (args->y_pos < args->height - 1) ? (args->y_pos + 1) : args->y_pos;
    //-------------
    int rand_num =  rand_interval(0,7);
    for (x; x < args->width; x++) {
        if (args->maze[x + args->width * y] == ' ') {
            args->maze[x + args->width * y] = randomletter;
            gotoxy(x,y);
            PRINTC (rand_num, "%c", args->maze[x + args->width * y]);
        } else {
            return;
        }
        char up_flag = args->maze[x + args->width * up];
        if (up_flag == ' ') {
            //printf(" CREATE_UP:%d %d", x, y);

            args->x_pos = x;
            args->y_pos = up;
            move_up(&args);
        }

        char down_flag = args->maze[x + args->width * down];
        if (down_flag == ' ') {
            //printf(" CREATE_DOWN:%d %d", x, y);
            args->x_pos = x;
            args->y_pos = down;
            move_down(&args);
        }
    }
}

void move_up(struct thread_args **argp) {
    struct thread_args *args = *argp;
    int x = args->x_pos;
    char randomletter = 'a' + (random() % 26);
    //-------------
    int left = (args->x_pos > 0) ? (args->x_pos - 1) : args->x_pos;
    int right = (args->x_pos < args->width - 1) ? (args->x_pos + 1) : args->x_pos;
    //-------------
    int rand_num =  rand_interval(0,7);
    for (unsigned y = args->y_pos+1; y-- > 0;) {
        if (args->maze[x + args->width * y] == ' ') {
            args->maze[x + args->width * y] = randomletter;
            gotoxy(x,y);
            PRINTC (rand_num, "%c", args->maze[x + args->width * y]);
        } else {
            //show_maze(maze, 8, 8);

            return;
        }
        char left_flag = args->maze[left + args->width * y];
        if (left_flag == ' ') {
            //printf(" CREATE_LEFT:%d %d", x, y);
            args->x_pos = left;
            args->y_pos = y;
            move_left(&args);
        }

        char right_flag = args->maze[right + args->width * y];
        if (right_flag == ' ') {
            //printf(" CREATE_RIGHT:%d %d", x, y);
            args->x_pos = right;
            args->y_pos = y;
            move_right(&args);
        }
    }
}

void move_left(struct thread_args **argp) {
    struct thread_args *args = *argp;
    int y = args->y_pos;
    char randomletter = 'a' + (random() % 26);
    //-------------
    int up = (args->y_pos > 0) ? (args->y_pos - 1) : args->y_pos;
    int down = (args->y_pos < args->height - 1) ? (args->y_pos + 1) : args->y_pos;
    //-------------
    int rand_num =  rand_interval(0,7);
    for (unsigned x = args->x_pos+1; x-- > 0;) {
        if (args->maze[x + args->width * y] == ' ') {
            args->maze[x + args->width * y] = randomletter;
            gotoxy(x,y);
            PRINTC (rand_num, "%c", args->maze[x + args->width * y]);
        } else {
            return;
        }
        char up_flag = args->maze[x + args->width * up];
        if (up_flag == ' ') {
            //printf(" CREATE_UP:%d %d", x, y);

            args->x_pos = x;
            args->y_pos = up;
            move_up(&args);
        }

        char down_flag = args->maze[x + args->width * down];
        if (down_flag == ' ') {
            //printf(" CREATE_DOWN:%d %d", x, y);
            args->x_pos = x;
            args->y_pos = down;
            move_down(&args);
        }
    }
}

void gotoxy(int x,int y){
    x+=1;
    y+=1;
    printf("%c[%d;%df",0x1B,y,x);
}



int main(int argc, char *argv[]) {
    /* Intializes random number generator */
    srand(time(0));

    int width = 8;
    int height = 8;


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
    //---------
    struct thread_args *args = malloc(sizeof *args);
    //args = (args_t *)malloc(1 * sizeof(args_t ));
    args->maze = bytes;
    args->width = width;
    args->height = height;
    args->x_pos = 0;
    args->y_pos = 0;
    move_down(&args);
    //move_right(bytes, 8, 8, 0, 7);
    //move_up(bytes, 8, 8, 4, 8); //(x_pos - 1) (y_pos - 1)
    //move_left(bytes, 8, 8, 2, 7); //(x_pos - 1) (y_pos - 1)
    //---printf("\n");
    //show_maze(bytes, 8, 8);
    printf("\n\n"); //Don't Remove
    free(bytes);
    return 0;
}
