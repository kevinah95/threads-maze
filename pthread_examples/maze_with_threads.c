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


typedef struct Threads {
    pthread_t *threads;
    int thread_count;
} thread_pool;

typedef struct arg_struct {
    char *maze;
    int width;
    int height;
    int x_pos;
    int y_pos;
}args_t;

thread_pool *tids;


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
void* show_maze(const char *maze, int width, int height) {
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

void* move_down(args_t *args) {
    int width = (args->width);
    int height = (args->height);
    int x_pos = (args->x_pos);
    int y_pos = (args->y_pos);
    int x = x_pos;
    int y = y_pos;
    char randomletter = 'a' + (random() % 26);
    //-------------
    int left = (x_pos > 0) ? (x_pos - 1) : x_pos;
    int right = (x_pos < width - 1) ? (x_pos + 1) : x_pos;
    //-------------
    int rand_num =  rand_interval(0,7);
    for (y; y < height; y++) {
        if (args->maze[x + width * y] == ' ') {
            args->maze[x + width * y] = randomletter;
            gotoxy(x,y);
            PRINTC (rand_num, "%c", args->maze[x + width * y]);
        } else {
            return NULL;
        }
        char left_flag = args->maze[left + width * y];
        if (left_flag == ' ') {
            //printf(" CREATE_LEFT:%d %d", x, y);
            move_left(args->maze, width, height, left, y);
        }

        char right_flag = args->maze[right + width * y];
        if (right_flag == ' ') {
            //printf(" CREATE_RIGHT:%d %d", x, y);
            move_right(args);
        }
    }
}

void* move_right(args_t *args) {
    int width = (args->width);
    int height = (args->height);
    int x_pos = (args->x_pos);
    int y_pos = (args->y_pos);
    int x = x_pos;
    int y = y_pos;
    char randomletter = 'a' + (random() % 26);
    //-------------
    int up = (y_pos > 0) ? (y_pos - 1) : y_pos;
    int down = (y_pos < height - 1) ? (y_pos + 1) : y_pos;
    //-------------
    int rand_num =  rand_interval(0,7);
    for (x; x < width; x++) {
        if (args->maze[x + width * y] == ' ') {
            args->maze[x + width * y] = randomletter;
            gotoxy(x,y);
            PRINTC (rand_num, "%c", args->maze[x + width * y]);
        } else {
            return NULL;
        }
        char up_flag = args->maze[x + width * up];
        if (up_flag == ' ') {
            //printf(" CREATE_UP:%d %d", x, y);
            move_up(args->maze, width, height, x, up);
        }

        char down_flag = args->maze[x + width * down];
        if (down_flag == ' ') {
            //printf(" CREATE_DOWN:%d %d", x, y);
            move_down(args);
        }
    }
}

void* move_up(char *maze, int *p_width, int *p_height, int *p_x_pos, int *p_y_pos) {
    int width = *(p_width);
    int height = *(p_height);
    int x_pos = *(p_x_pos);
    int y_pos = *(p_y_pos);
    int x = x_pos;
    char randomletter = 'a' + (random() % 26);
    //-------------
    int left = (x_pos > 0) ? (x_pos - 1) : x_pos;
    int right = (x_pos < width - 1) ? (x_pos + 1) : x_pos;
    //-------------
    int rand_num =  rand_interval(0,7);
    for (unsigned y = y_pos+1; y-- > 0;) {
        if (maze[x + width * y] == ' ') {
            maze[x + width * y] = randomletter;
            gotoxy(x,y);
            PRINTC (rand_num, "%c", maze[x + width * y]);
        } else {
            //show_maze(maze, 8, 8);

            return NULL;
        }
        char left_flag = maze[left + width * y];
        if (left_flag == ' ') {
            //printf(" CREATE_LEFT:%d %d", x, y);
            move_left(maze, width, height, left, y);
        }

        char right_flag = maze[right + width * y];
        if (right_flag == ' ') {
            //printf(" CREATE_RIGHT:%d %d", x, y);
            move_right(maze, width, height, right, y);
        }
    }
}

void* move_left(char *maze, int *p_width, int *p_height, int *p_x_pos, int *p_y_pos) {
    int width = *(p_width);
    int height = *(p_height);
    int x_pos = *(p_x_pos);
    int y_pos = *(p_y_pos);
    int y = y_pos;
    char randomletter = 'a' + (random() % 26);
    //-------------
    int up = (y_pos > 0) ? (y_pos - 1) : y_pos;
    int down = (y_pos < height - 1) ? (y_pos + 1) : y_pos;
    //-------------
    int rand_num =  rand_interval(0,7);
    for (unsigned x = x_pos+1; x-- > 0;) {
        if (maze[x + width * y] == ' ') {
            maze[x + width * y] = randomletter;
            gotoxy(x,y);
            PRINTC (rand_num, "%c", maze[x + width * y]);
        } else {
            return NULL;
        }
        char up_flag = maze[x + width * up];
        if (up_flag == ' ') {
            //printf(" CREATE_UP:%d %d", x, y);
            move_up(maze, width, height, x, up);
        }

        char down_flag = maze[x + width * down];
        if (down_flag == ' ') {
            //printf(" CREATE_DOWN:%d %d", x, y);
            move_down(maze, width, height, x, down);
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

    int err;
    int width = 0;
    int height = 0;
    int x = 0;
    int y = 0;


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

    //----threads
    tids=(thread_pool *)malloc(1 * sizeof(thread_pool ));
    tids->threads=(pthread_t *)malloc(1 * sizeof(pthread_t ));
    tids->thread_count = 0;

    tids->threads =(pthread_t *)realloc(tids->threads,sizeof ( pthread_t )*(tids->thread_count+1)) ;
    if(tids==NULL)
        printf("error with realloc");
    args_t *args;
    args->maze = bytes;
    args->width = width;
    args->height = height;
    args->x = x;
    args->y = y;
    err = pthread_create(&(tids->threads[tids->thread_count]), NULL, &move_down,args);
    if (err != 0)
        printf("\ncan't create thread :[%s]", strerror(err));
    else
        printf("\n Thread %d created successfully\n",tids->thread_count);

    tids->thread_count++;

    show_maze(bytes, 8, 8);
    //move_down(bytes, 8, 8, 0, 0);

    int i = 0;
    while(i < tids->thread_count)
    {
        pthread_join(tids->threads[i], NULL);
        pthread_join(tids->threads[i], NULL);
        i++;
    }

    printf("\n\n"); //Don't Remove
    free(bytes);
    return 0;
}
