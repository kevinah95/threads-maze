/* Maze generator in C.
 * Joe Wingbermuehle
 * 19990805
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>

/* Display the maze. */
void ShowMaze(const char *maze, int width, int height) {
   int x, y;
   for(y = 0; y < height; y++) {
      for(x = 0; x < width; x++) {
          //if(isprint(maze[y * width + x]))
            printf("%c",maze[y * width + x]);
      }
      printf("\n");//each row
   }
   printf("\n");
}

long slurp(char const* path, char **buf, bool add_nul)
{
    FILE  *fp;
    size_t fsz;
    long   off_end;
    int    rc;

    /* Open the file */
    fp = fopen(path, "rb");
    if( NULL == fp ) {
        return -1L;
    }

    /* Seek to the end of the file */
    rc = fseek(fp, 0L, SEEK_END);
    if( 0 != rc ) {
        return -1L;
    }

    /* Byte offset to the end of the file (size) */
    if( 0 > (off_end = ftell(fp)) ) {
        return -1L;
    }
    fsz = (size_t)off_end;

    /* Allocate a buffer to hold the whole file */
    *buf = malloc( fsz+(int)add_nul );
    if( NULL == *buf ) {
        return -1L;
    }

    /* Rewind file pointer to start of file */
    rewind(fp);

    /* Slurp file into buffer */
    if( fsz != fread(*buf, 1, fsz, fp) ) {
        free(*buf);
        return -1L;
    }

    /* Close the file */
    if( EOF == fclose(fp) ) {
        free(*buf);
        return -1L;
    }

    if( add_nul ) {
        /* Make sure the buffer is NUL-terminated, just in case */
        buf[fsz] = '\0';
    }

    /* Return the file size */
    return (long)fsz;
}



int main(int argc,char *argv[]) {

   int width, height;
   char *maze;
    long  file_size;

   printf("Maze by Joe Wingbermuehle 19990805\n");
   if(argc != 3 && argc != 4) {
      printf("usage: maze <width> <height> [s]\n");
      exit(EXIT_FAILURE);
   }

   /* Get and validate the size. */
   width = atoi(argv[1]);
   height = atoi(argv[2]);
   if(width < 2 || height < 2) {
      printf("error: illegal maze size\n");
      exit(EXIT_FAILURE);
   }




   /* Allocate the maze array. */
   maze = (char*)malloc(width * height * sizeof(char));
   if(maze == NULL) {
      printf("error: not enough memory\n");
      exit(EXIT_FAILURE);
   }

    /* Try the first command-line argument as a file name */
    FILE* f = fopen(argv[3], "r");
    int n = 0, i = 0;
    int numbers[5]; // assuming there are only 5 numbers in the file

    fscanf(f, "%c", &maze);



    //file_size = slurp(argv[3], &maze, false);

    /* Bail if we get a negative file size back from slurp() */

    printf("%c", &maze);
    /* Write to stdout whatever slurp() read in */
    printf("\n\n\n");
    //ShowMaze(maze, width, height);

   /* Generate and display the maze. *//*
   GenerateMaze(maze, width, height);
   ShowMaze(maze, width, height);

   *//* Solve the maze if requested. *//*
   if(argc == 4) {
      SolveMaze(maze, width, height);
      ShowMaze(maze, width, height);
   }*/

   /* Clean up. */
   free(maze);
    fclose(f);
   exit(EXIT_SUCCESS);

}

