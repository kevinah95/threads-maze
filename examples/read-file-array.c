
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    const char *s = "";
    char *token = NULL;

    int i = 0;
    char arr[200];
    int j;

    fp = fopen("examples/maze.txt", "r");
    if (fp == NULL) {
        printf("Error opening");
        exit(EXIT_FAILURE);
    }

    while ((read = getline(&line, &len, fp)) != -1) {
        token = strtok(line, s);

        while (token != NULL) {
            arr[i] = token;
            printf("%f\n", arr[i]);
            token = strtok(NULL, s);
            i++;
        }
    }

    exit(EXIT_SUCCESS);
    return 0;
}