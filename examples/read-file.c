#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

    /* check args */
    if (argc != 2) {
        printf("usage: %s <maze_file>\n", argv[0]);
        return 2;
    }

    FILE *f;
    char c;
    f=fopen(argv[1],"rt");

    while((c=fgetc(f))!=EOF){
        printf("%c",c);
    }

    if(!1)
        printf("dasd");

    fclose(f);
    return 0;
}
