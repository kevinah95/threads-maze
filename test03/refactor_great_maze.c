#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

/* Have problems using posix barriers when symbol value is 200112L */
/* This requires more investigation, but will work for the moment */
/*#if defined(_POSIX_BARRIERS) && ( (_POSIX_BARRIERS - 20012L) >= 0 && _POSIX_BARRIERS != 200112L)
#define _POSIX_BARRIERS_MINE		  #define _POSIX_BARRIERS_MINE
#endif		  #endi*/
#define MILLISECOND 1000
#define SECOND 1000*MILLISECOND //1000*MILLISECOND <- One Second

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

typedef struct Threads {
    pthread_t *threads;
    int thread_count;
} thread_pool;

thread_pool *tids;
pthread_mutex_t lock;



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
void *move_down(struct thread_args **argp);
void *move_right(struct thread_args **argp);
void *move_left(struct thread_args **argp);
void *move_up(struct thread_args **argp);



void *move_down(struct thread_args **argp) {

    struct thread_args *args = *argp;
    int x = args->x_pos;
    int y = args->y_pos;
    int err;
    char randomletter = 'a' + (random() % 26);
    //-------------
    int left = (args->x_pos > 0) ? (args->x_pos - 1) : args->x_pos;
    int right = (args->x_pos < args->width - 1) ? (args->x_pos + 1) : args->x_pos;
    //-------------
    int rand_num =  rand_interval(0,7);
    //pthread_mutex_lock(&lock);
    for (y; y < args->height; y++) {
        //pthread_mutex_lock(&lock);
        if (args->maze[x + args->width * y] == ' ') {
            args->maze[x + args->width * y] = randomletter;
            //pthread_mutex_lock(&lock);
            gotoxy(x,y);
            PRINTC (rand_num, "%c", args->maze[x + args->width * y]);
            fflush(stdout);
            usleep(SECOND);
            //pthread_mutex_unlock(&lock);
        } else {

            return NULL;
        }
        //pthread_mutex_unlock(&lock);

        char left_flag = args->maze[left + args->width * y];
        if (left_flag == ' ') {
            //printf(" CREATE_LEFT:%d %d", x, y);
            args->x_pos = left;
            args->y_pos = y;
            //move_left(&args);
            //pthread_mutex_lock(&lock);
            //usleep(SECOND);
            tids->threads =(pthread_t *)realloc(tids->threads,sizeof ( pthread_t )*(tids->thread_count+1)) ;
            if(tids==NULL)
                printf("error with realloc");

            struct thread_args *args_copy = malloc(sizeof *args);
            args_copy->maze = args->maze;
            args_copy->x_pos = args->x_pos;
            args_copy->y_pos = args->y_pos;
            args_copy->width = args->width;
            args_copy->height = args->height;
            memcpy(args_copy,args,sizeof(args));
            err = pthread_create(&(tids->threads[tids->thread_count]), NULL, &move_left, &args_copy);
            if (err != 0)
                printf("\ncan't create thread :[%s]", strerror(err));
            /*else
                printf("\n Thread %d created successfully\n",tids->thread_count);*/
            tids->thread_count++;
            //pthread_join(tids->threads[tids->thread_count], NULL);
            //pthread_mutex_unlock(&lock);
        }


        char right_flag = args->maze[right + args->width * y];
        if (right_flag == ' ') {
            //printf(" CREATE_RIGHT:%d %d", x, y);
            args->x_pos = right;
            args->y_pos = y;
            //move_right(&args);

            //pthread_mutex_lock(&lock);
            //usleep(SECOND);
            tids->threads =(pthread_t *)realloc(tids->threads,sizeof ( pthread_t )*(tids->thread_count+1)) ;
            if(tids==NULL)
                printf("error with realloc");
            struct thread_args *args_copy = malloc(sizeof *args);
            memcpy(args_copy,args,sizeof(args));
            //strncpy(args_copy->maze, args->maze, sizeof(args->maze));
            args_copy->maze = args->maze;
            args_copy->x_pos = args->x_pos;
            args_copy->y_pos = args->y_pos;
            args_copy->width = args->width;
            args_copy->height = args->height;
            err = pthread_create(&(tids->threads[tids->thread_count]), NULL, &move_right, &args_copy);
            if (err != 0)
                printf("\ncan't create thread :[%s]", strerror(err));
            /*else
                printf("\n Thread %d created successfully\n",tids->thread_count);*/
            tids->thread_count++;
            //pthread_join(tids->threads[tids->thread_count], NULL);
            //pthread_mutex_unlock(&lock);
        }

    }
    //pthread_mutex_unlock(&lock);





    return NULL;
}

void *move_right(struct thread_args **argp) {

    struct thread_args *args = *argp;
    int x = args->x_pos;
    int y = args->y_pos;
    int err;
    char randomletter = 'a' + (random() % 26);
    //-------------
    int up = (args->y_pos > 0) ? (args->y_pos - 1) : args->y_pos;
    int down = (args->y_pos < args->height - 1) ? (args->y_pos + 1) : args->y_pos;
    //-------------
    int rand_num =  rand_interval(0,7);
    //pthread_mutex_lock(&lock);
    for (x; x < args->width; x++) {
        //pthread_mutex_lock(&lock);
        if (args->maze[x + args->width * y] == ' ') {
            args->maze[x + args->width * y] = randomletter;
            //pthread_mutex_lock(&lock);
            gotoxy(x,y);
            PRINTC (rand_num, "%c", args->maze[x + args->width * y]);
            fflush(stdout);
            usleep(SECOND);
            //pthread_mutex_unlock(&lock);
        } else {
            return NULL;
        }
        //pthread_mutex_unlock(&lock);

        char up_flag = args->maze[x + args->width * up];
        if (up_flag == ' ') {
            //printf(" CREATE_UP:%d %d", x, y);

            args->x_pos = x;
            args->y_pos = up;
            //move_up(&args);
            //pthread_mutex_lock(&lock);
            tids->threads =(pthread_t *)realloc(tids->threads,sizeof ( pthread_t )*(tids->thread_count+1)) ;
            if(tids==NULL)
                printf("error with realloc");
            //usleep(SECOND);
            struct thread_args *args_copy = malloc(sizeof *args);
            args_copy->maze = args->maze;
            args_copy->x_pos = args->x_pos;
            args_copy->y_pos = args->y_pos;
            args_copy->width = args->width;
            args_copy->height = args->height;
            memcpy(args_copy,args,sizeof(args));
            err = pthread_create(&(tids->threads[tids->thread_count]), NULL, &move_up, &args_copy);
            if (err != 0)
                printf("\ncan't create thread :[%s]", strerror(err));
            /*else
                printf("\n Thread %d created successfully\n",tids->thread_count);*/
            tids->thread_count++;
            //pthread_join(tids->threads[tids->thread_count], NULL);
            //pthread_mutex_unlock(&lock);
        }
        char down_flag = args->maze[x + args->width * down];
        if (down_flag == ' ') {
            //printf(" CREATE_DOWN:%d %d", x, y);
            args->x_pos = x;
            args->y_pos = down;
            //move_down(&args);
            //pthread_mutex_lock(&lock);
            tids->threads =(pthread_t *)realloc(tids->threads,sizeof ( pthread_t )*(tids->thread_count+1)) ;
            if(tids==NULL)
                printf("error with realloc");

            //usleep(SECOND);
            struct thread_args *args_copy = malloc(sizeof *args);
            args_copy->maze = args->maze;
            args_copy->x_pos = args->x_pos;
            args_copy->y_pos = args->y_pos;
            args_copy->width = args->width;
            args_copy->height = args->height;
            memcpy(args_copy,args,sizeof(args));
            err = pthread_create(&(tids->threads[tids->thread_count]), NULL, &move_down, &args_copy);
            if (err != 0)
                printf("\ncan't create thread :[%s]", strerror(err));
            /*else
                printf("\n Thread %d created successfully\n",tids->thread_count);*/
            tids->thread_count++;
            //pthread_join(tids->threads[tids->thread_count], NULL);
            //pthread_mutex_unlock(&lock);
        }

    }

    //pthread_mutex_unlock(&lock);


    return NULL;
}

void *move_up(struct thread_args **argp) {

    struct thread_args *args = *argp;
    int x = args->x_pos;
    int err;
    char randomletter = 'a' + (random() % 26);
    //-------------
    int left = (args->x_pos > 0) ? (args->x_pos - 1) : args->x_pos;
    int right = (args->x_pos < args->width - 1) ? (args->x_pos + 1) : args->x_pos;
    //-------------
    int rand_num =  rand_interval(0,7);
    //pthread_mutex_lock(&lock);
    for (unsigned y = args->y_pos+1; y-- > 0;) {
        //pthread_mutex_lock(&lock);
        if (args->maze[x + args->width * y] == ' ') {
            args->maze[x + args->width * y] = randomletter;
            //pthread_mutex_lock(&lock);
            gotoxy(x,y);
            PRINTC (rand_num, "%c", args->maze[x + args->width * y]);
            fflush(stdout);
            usleep(SECOND);
            //pthread_mutex_unlock(&lock);
        } else {
            //show_maze(maze, 8, 8);

            return NULL;
        }
        //pthread_mutex_unlock(&lock);

        char left_flag = args->maze[left + args->width * y];
        if (left_flag == ' ') {
            //printf(" CREATE_LEFT:%d %d", x, y);
            args->x_pos = left;
            args->y_pos = y;
            //move_left(&args);
            //pthread_mutex_lock(&lock);
            tids->threads =(pthread_t *)realloc(tids->threads,sizeof ( pthread_t )*(tids->thread_count+1)) ;
            if(tids==NULL)
                printf("error with realloc");

            //usleep(SECOND);

            struct thread_args *args_copy = malloc(sizeof *args);
            args_copy->maze = args->maze;
            args_copy->x_pos = args->x_pos;
            args_copy->y_pos = args->y_pos;
            args_copy->width = args->width;
            args_copy->height = args->height;
            memcpy(args_copy,args,sizeof(args));
            err = pthread_create(&(tids->threads[tids->thread_count]), NULL, &move_left, &args_copy);
            if (err != 0)
                printf("\ncan't create thread :[%s]", strerror(err));
            /*else
                printf("\n Thread %d created successfully\n",tids->thread_count);*/
            tids->thread_count++;
            //pthread_join(tids->threads[tids->thread_count], NULL);
            //pthread_mutex_unlock(&lock);
        }

        char right_flag = args->maze[right + args->width * y];
        if (right_flag == ' ') {
            //printf(" CREATE_RIGHT:%d %d", x, y);
            args->x_pos = right;
            args->y_pos = y;
            //move_right(&args);
            //pthread_mutex_lock(&lock);
            tids->threads =(pthread_t *)realloc(tids->threads,sizeof ( pthread_t )*(tids->thread_count+1)) ;
            if(tids==NULL)
                printf("error with realloc");

            //usleep(SECOND);
            struct thread_args *args_copy = malloc(sizeof *args);
            args_copy->maze = args->maze;
            args_copy->x_pos = args->x_pos;
            args_copy->y_pos = args->y_pos;
            args_copy->width = args->width;
            args_copy->height = args->height;
            memcpy(args_copy,args,sizeof(args));
            err = pthread_create(&(tids->threads[tids->thread_count]), NULL, &move_right, &args_copy);
            if (err != 0)
                printf("\ncan't create thread :[%s]", strerror(err));
            /*else
                printf("\n Thread %d created successfully\n",tids->thread_count);*/
            tids->thread_count++;
            //pthread_join(tids->threads[tids->thread_count], NULL);
            //pthread_mutex_unlock(&lock);
        }

    }
    //pthread_mutex_unlock(&lock);


    return NULL;
}

void *move_left(struct thread_args **argp) {

    struct thread_args *args = *argp;
    int y = args->y_pos;
    int err;
    char randomletter = 'a' + (random() % 26);
    //-------------
    int up = (args->y_pos > 0) ? (args->y_pos - 1) : args->y_pos;
    int down = (args->y_pos < args->height - 1) ? (args->y_pos + 1) : args->y_pos;
    //-------------
    int rand_num =  rand_interval(0,7);
    //pthread_mutex_lock(&lock);
    for (unsigned x = args->x_pos+1; x-- > 0;) {
        //pthread_mutex_lock(&lock);
        if (args->maze[x + args->width * y] == ' ') {
            args->maze[x + args->width * y] = randomletter;
            //pthread_mutex_lock(&lock);
            gotoxy(x,y);
            PRINTC (rand_num, "%c", args->maze[x + args->width * y]);
            fflush(stdout);
            usleep(SECOND);
            //pthread_mutex_unlock(&lock);
        } else {

            return NULL;
        }
        //pthread_mutex_unlock(&lock);

        char up_flag = args->maze[x + args->width * up];
        if (up_flag == ' ') {
            //printf(" CREATE_UP:%d %d", x, y);

            args->x_pos = x;
            args->y_pos = up;
            //move_up(&args);
            //pthread_mutex_lock(&lock);
            tids->threads =(pthread_t *)realloc(tids->threads,sizeof ( pthread_t )*(tids->thread_count+1)) ;
            if(tids==NULL)
                printf("error with realloc");


            //usleep(SECOND);
            struct thread_args *args_copy = malloc(sizeof *args);
            args_copy->maze = args->maze;
            args_copy->x_pos = args->x_pos;
            args_copy->y_pos = args->y_pos;
            args_copy->width = args->width;
            args_copy->height = args->height;
            memcpy(args_copy,args,sizeof(args));
            err = pthread_create(&(tids->threads[tids->thread_count]), NULL, &move_up, &args_copy);
            if (err != 0)
                printf("\ncan't create thread :[%s]", strerror(err));
            /*else
                printf("\n Thread %d created successfully\n",tids->thread_count);*/
            tids->thread_count++;
            //pthread_join(tids->threads[tids->thread_count], NULL);
            //pthread_mutex_unlock(&lock);
        }

        char down_flag = args->maze[x + args->width * down];
        if (down_flag == ' ') {
            //printf(" CREATE_DOWN:%d %d", x, y);
            args->x_pos = x;
            args->y_pos = down;
            //move_down(&args);
            //pthread_mutex_lock(&lock);
            tids->threads =(pthread_t *)realloc(tids->threads,sizeof ( pthread_t )*(tids->thread_count+1)) ;
            if(tids==NULL)
                printf("error with realloc");

            //usleep(SECOND);
            struct thread_args *args_copy = malloc(sizeof *args);
            args_copy->maze = args->maze;
            args_copy->x_pos = args->x_pos;
            args_copy->y_pos = args->y_pos;
            args_copy->width = args->width;
            args_copy->height = args->height;
            memcpy(args_copy,args,sizeof(args));
            err = pthread_create(&(tids->threads[tids->thread_count]), NULL, &move_down, &args_copy);
            if (err != 0)
                printf("\ncan't create thread :[%s]", strerror(err));
            /*else
                printf("\n Thread %d created successfully\n",tids->thread_count);*/
            tids->thread_count++;
            //pthread_join(tids->threads[tids->thread_count], NULL);
            //pthread_mutex_unlock(&lock);
        }

    }
    //pthread_mutex_unlock(&lock);


    return NULL;
}

void gotoxy(int x,int y){
    x+=1;
    y+=1;
    printf("%c[%d;%df",0x1B,y,x);
}



//int main(int argc, char *argv[]) {
int main(void ) {
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
    int err;

    tids=(thread_pool *)malloc(1 * sizeof(thread_pool ));
    tids->threads=(pthread_t *)malloc(1 * sizeof(pthread_t ));
    tids->thread_count = 0;
    tids->threads =(pthread_t *)realloc(tids->threads,sizeof ( pthread_t )*(tids->thread_count+1)) ;
    if(tids==NULL)
        printf("error with realloc");

    //MUTEX
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    //

    //----------
    struct thread_args *args = malloc(sizeof *args);
    //args = (args_t *)malloc(1 * sizeof(args_t ));
    args->maze = bytes;
    args->width = width;
    args->height = height;
    args->x_pos = 0;
    args->y_pos = 0;
    //move_down(&args);
    //pthread_mutex_lock(&lock); // Stop thread from getting the mutex
    err = pthread_create(&(tids->threads[tids->thread_count]), NULL, &move_down, &args);
    if (err != 0)
        printf("\ncan't create thread :[%s]", strerror(err));
    /*else
        printf("\n Thread %d created successfully\n",tids->thread_count);*/
    tids->thread_count++;
    //pthread_mutex_unlock(&lock); // Let the thread run for a while
    //pthread_join(tids->threads[tids->thread_count], NULL);

    //move_right(bytes, 8, 8, 0, 7);
    //move_up(bytes, 8, 8, 4, 8); //(x_pos - 1) (y_pos - 1)
    //move_left(bytes, 8, 8, 2, 7); //(x_pos - 1) (y_pos - 1)
    //---printf("\n");
    //show_maze(bytes, 8, 8);
    printf("\n\n"); //Don't Remove
    int i = 0;
    while(i < tids->thread_count)
    {
        pthread_join(tids->threads[i], NULL);
        i++;
    }
    pthread_mutex_destroy(&lock);
    //show_maze(bytes, 8, 8);
    free(bytes);
    return 0;
}
