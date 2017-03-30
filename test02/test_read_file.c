#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define PRINTC(c,f,s) printf ("\033[%dm" f "\033[0m", 30 + c, s)

/* Display the maze. */
void show_maze(const char *maze, int width, int height) {
    int x, y;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            printf("%c", maze[x + width * y]);
            /*switch(maze[y * width + x]) {
                case '#':  printf("[]");  break;
                case ' ':  printf("  ");  break;
                default: printf("<>");  break;
            }*/
        }
        printf("\n");//each row
    }
    printf("\n");
}
unsigned int rand_interval(unsigned int min, unsigned int max)
{
    int r;
    const unsigned int range = 1 + max - min;
    const unsigned int buckets = RAND_MAX / range;
    const unsigned int limit = buckets * range;

    /* Create equal size buckets all in a row, then fire randomly towards
     * the buckets until you land in one of them. All buckets are equally
     * likely. If you land off the end of the line of buckets, try again. */
    do
    {
        r = rand();
    } while (r >= limit);

    return min + (r / buckets);
}


void move_down(char *maze, int width, int height, int x_pos, int y_pos){
    int x = x_pos;
    int y = y_pos;

    for (y; y < height; y++) {
        //printf("%c", maze[x + width * y]);
        //printf("\n");
        if(maze[x+width*y]==' '){
            maze[x + width * y]='x';

        } else{
            //die
        }

    }
}

void move_right(char *maze, int width, int height, int x_pos, int y_pos){
    int x = x_pos;
    int y = y_pos;
    for (x; x < width; x++) {
        //printf("%c", maze[x + width * y]);
        //printf("\n");
        maze[x + width * y]='x';
    }
}

void move_up(char *maze, int width, int height, int x_pos, int y_pos){
    int x = x_pos;
    //int y = y_pos;
    for (unsigned y = height ; y-- > 0 ;) {
        //printf("%c", maze[x + width * y]);
        //printf("\n");
        maze[x + width * y]='x';
    }
}

void move_left(char *maze, int width, int height, int x_pos, int y_pos){
    //int x = x_pos;
    int y = y_pos;
    for (unsigned x = width ; x-- > 0 ;) {
        //printf("%c", maze[x + width * y]);
        //printf("\n");
        maze[x + width * y]='x';
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
    move_right(bytes, 8, 8, 0, 0);
    move_up(bytes, 8, 8, 7, 7); //(x_pos - 1) (y_pos - 1)
    move_left(bytes, 8, 8, 7, 7); //(x_pos - 1) (y_pos - 1)
    printf("\n");
    show_maze(bytes, 8, 8);
    //-------------------------Colors!!
    /*PRINTC (rand_interval(0,7), "%s\n", "bar");
    PRINTC (rand_interval(10,17), "%c", 'a');*/

    free(bytes);
    return 0;
    /*free(temp); // free allocated memory
    free(bytes); // free allocated memory*/
}
