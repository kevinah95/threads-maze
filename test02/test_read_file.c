#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>


#define MILLISECOND 1000
#define SECOND 1000*MILLISECOND //1000*MILLISECOND <- One Second

#define PRINTC(c, f, s0, s1) printf ("\033[%dm" f "\033[0m", 30 + c, s0,s1)


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

    for (y = 0; y < height; y++) {
        //int rand_num =  rand_interval(0,7);
        for (x = 0; x < width; x++) {
            printf("%c", maze[x + width * y]);
            fflush(stdout);
            usleep(SECOND);
            /*switch(maze[y * width + x]) {
                case '#':  printf("[]");  break;
                case ' ':  printf("  ");  break;
                //default: printf("%c%c", maze[x + width * y],maze[x + width * y]);  break;
                default: PRINTC (rand_num, "%c%c", maze[x + width * y],maze[x + width * y]);  break;
            }*/
        }
        printf("\n");//each row
    }

    printf("\n");
}

void move_down(char *maze, int width, int height, int x_pos, int y_pos) {
    int x = x_pos;
    int y = y_pos;
    char randomletter = 'a' + (random() % 26);
    //-------------
    int left = (x_pos > 0) ? (x_pos - 1) : x_pos;
    int right = (x_pos < width - 1) ? (x_pos + 1) : x_pos;
    //-------------
    printf("\n");
    for (y; y < height; y++) {
        //printf("%c", maze[x + width * y]);
        //printf("\n");

        if (maze[x + width * y] == ' ') {
            maze[x + width * y] = randomletter;
        } else {
            show_maze(maze, 8, 8);

            return;
        }
        char left_flag = maze[left + width * y];
        if (left_flag == ' ') {
            printf(" CREATE_LEFT:%d %d", x, y);
            move_left(maze, width, height, left, y);
        }

        char right_flag = maze[right + width * y];
        if (right_flag == ' ') {
            printf(" CREATE_RIGHT:%d %d", x, y);
            move_right(maze, width, height, right, y);
        }


    }
}

void move_right(char *maze, int width, int height, int x_pos, int y_pos) {
    int x = x_pos;
    int y = y_pos;
    char randomletter = 'a' + (random() % 26);
    //-------------
    int up = (y_pos > 0) ? (y_pos - 1) : y_pos;
    int down = (y_pos < height - 1) ? (y_pos + 1) : y_pos;
    //-------------
    printf("\n");
    for (x; x < width; x++) {
        //printf("%c", maze[x + width * y]);
        //printf("\n");
        if (maze[x + width * y] == ' ') {
            maze[x + width * y] = randomletter;
        } else {
            show_maze(maze, 8, 8);
            
            return;
        }
        char up_flag = maze[x + width * up];
        if (up_flag == ' ') {
            printf(" CREATE_UP:%d %d", x, y);
            move_up(maze, width, height, x, up);
        }

        char down_flag = maze[x + width * down];
        if (down_flag == ' ') {
            printf(" CREATE_DOWN:%d %d", x, y);
            move_down(maze, width, height, x, down);
        }
    }
}

void move_up(char *maze, int width, int height, int x_pos, int y_pos) {
    int x = x_pos;
    //int y = y_pos;
    char randomletter = 'a' + (random() % 26);
    //-------------
    int left = (x_pos > 0) ? (x_pos - 1) : x_pos;
    int right = (x_pos < width - 1) ? (x_pos + 1) : x_pos;
    //-------------
    printf("\n");
    for (unsigned y = y_pos+1; y-- > 0;) {
        //printf("%c", maze[x + width * y]);
        //printf("\n");
        //printf("Y:%d",y);
        if (maze[x + width * y] == ' ') {
            maze[x + width * y] = randomletter;
        } else {
            show_maze(maze, 8, 8);
            sleep(1);
            return;
        }
        char left_flag = maze[left + width * y];
        if (left_flag == ' ') {
            printf(" CREATE_LEFT:%d %d", x, y);
            move_left(maze, width, height, left, y);
        }

        char right_flag = maze[right + width * y];
        if (right_flag == ' ') {
            printf(" CREATE_RIGHT:%d %d", x, y);
            move_right(maze, width, height, right, y);
        }
    }
}

void move_left(char *maze, int width, int height, int x_pos, int y_pos) {
    //int x = x_pos;
    int y = y_pos;
    char randomletter = 'a' + (random() % 26);
    //-------------
    int up = (y_pos > 0) ? (y_pos - 1) : y_pos;
    int down = (y_pos < height - 1) ? (y_pos + 1) : y_pos;
    //-------------
    printf("\n");
    for (unsigned x = x_pos+1; x-- > 0;) {
        //printf("%c", maze[x + width * y]);
        //printf("\n");
        if (maze[x + width * y] == ' ') {
            maze[x + width * y] = randomletter;
        } else {
            show_maze(maze, 8, 8);
            sleep(1);
            return;
        }
        char up_flag = maze[x + width * up];
        if (up_flag == ' ') {
            printf(" CREATE_UP:%d %d", x, y);
            move_up(maze, width, height, x, up);
        }

        char down_flag = maze[x + width * down];
        if (down_flag == ' ') {
            printf(" CREATE_DOWN:%d %d", x, y);
            move_down(maze, width, height, x, down);
        }
    }
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

    printf("String:");
    printf("%s", bytes);
    printf("Len: ");
    printf("%d", strlen(bytes));
    move_down(bytes, 8, 8, 0, 0);
    //move_right(bytes, 8, 8, 0, 7);
    //move_up(bytes, 8, 8, 4, 8); //(x_pos - 1) (y_pos - 1)
    //move_left(bytes, 8, 8, 2, 7); //(x_pos - 1) (y_pos - 1)
    printf("\n");
    //show_maze(bytes, 8, 8);
    //-------------------------Colors!!
    /*PRINTC (rand_interval(0,7), "%s\n", "bar");
    PRINTC (rand_interval(10,17), "%c", 'a');*/

    free(bytes);
    return 0;
    /*free(temp); // free allocated memory
    free(bytes); // free allocated memory*/
}
