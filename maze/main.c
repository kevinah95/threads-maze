#include <stdio.h>
#include "vector.c"


int main() {
    //Lectura de la dirección del archivo
    char dir [256];
    printf("Ingrese la dirección del archivo con el laberinto:\n>>");
    gets(dir);

    //Lectura del archivo
    FILE *fp;
    char c;
    int n = 0;

    fp = fopen(dir,"r");
    if(fp == NULL)
    {
        perror("Error in opening file");
        return(-1);
    }

    //Instanciación de las variables numRows (height) y numCols (width)
    int numRows = 0;
    int numCols = 0;
    Vector vector;
    vector_init(&vector);
    //Funciones del vector
    //vector_append(&vector, 'a');
    //vector_get(&vector, 5));
    //vector_free(&vector);

    do
    {
        c = fgetc(fp);
        if( feof(fp) )
        {
            break ;
        }
        if (n == 0){
            //Aquí se obtiene la cantidad de filas
            printf("Filas: ");
            numRows = c - '0';
            printf("%d",numRows);

        }
        else if (n == 2){
            //Aquí se obtiene la cantidad de columnas
            printf(" Columnas: ");
            numCols = c - '0';
            printf("%d",numCols);
            //char mazeArray[numRows,numCols] = { {"0" } };
        }
        else{
            //Se agregan los demás caracteres al vector
            if (c != '\n'){
                vector_append(&vector, c);
                //printf("%c",c);
            }

        }
        n++;
    }while(1);

    //printf("primer elemento del vector:");
    //printf("%c",vector_get(&vector, 0));
    int size = numCols * numRows;
    for (int i = 0 ; i < size ; i++){
        printf("%c",vector_get(&vector, i));
    }
    fclose(fp);

    return(0);
}