#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>




int main(int argc, char *argv[])
{

    FILE *f = fopen("examples/maze.txt", "rb");
    fseek(f, 0, SEEK_END);
    long pos = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *bytes = malloc(pos);
    //fread(bytes, pos, 1, f);
    //fgets(bytes,pos,f);
    //fscanf(f,"%c*",bytes);
    fscanf(f, "%*c %[^\\n\\r]", bytes);

    fclose(f);
    //bytes[strcspn(bytes, "\n")] = ' ';
    char* temp;
    // Remove \n.
    while ((temp = strstr(bytes, "\n")) != NULL) {
    // Len is the length of the string, from the ampersand \n, including the \n.
        int len = strlen(bytes);
        memcpy(temp, temp + 1, len);
    }
    printf("String: ");
    printf("%s",bytes);
    printf("Len: ");
    printf("%d",strlen(bytes));


    free(bytes);
    return 0;
    /*free(temp); // free allocated memory
    free(bytes); // free allocated memory*/
}
