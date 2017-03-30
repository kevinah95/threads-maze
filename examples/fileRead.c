#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

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

    //generacion de una matriz que representa el laberinto
    int numRows = 0;
    int numCols = 0;
    char temp = "";
    int curRow = 0;
    int curCol = 0;

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
            printf("Columnas: ");
            numCols = c - '0';
            printf("%d",numCols);
            //char mazeArray[numRows,numCols] = { {"0" } };
        }
        else{
            //En esta parte se deberían agregar los caracteresa un arreglo
            printf("%c",c);
        }
        n++;
    }while(1);

    fclose(fp);

    return(0);
}