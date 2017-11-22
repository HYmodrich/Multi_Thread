#include<stdio.h>
#include<stdlib.h>

#define MATRIX_SIZE 4000

int** readMatrix(char* fileName)
{
    FILE *fp;
    int** matrix;

    matrix =malloc(sizeof(int*)*MATRIX_SIZE);
    int i=0;
    int j=0;
    for(i=0;i<MATRIX_SIZE;i++)
    { 
        matrix[i]=malloc(sizeof(int)*MATRIX_SIZE);
        if(matrix[i] == NULL)
        {
            fprintf(stderr, "out of memory\n");
            exit(0);
        }
    }

    fp = fopen(fileName,"r");
    for(int k=0;k<MATRIX_SIZE;k++)
    {
        for(j=0;j<MATRIX_SIZE;j++)
        {
          int test;
          fscanf(fp,"%d",&test);
          matrix[k][j] = test;
        }
    }
    fclose(fp);
    return matrix;
}

unsigned long long ** matrixMultiply(int** matrix1, int** matrix2)
{
    unsigned long long** result = (unsigned long long**)malloc(sizeof(unsigned long long*)*MATRIX_SIZE);
    for(int i=0;i<MATRIX_SIZE;i++)
    {
        result[i] = malloc(sizeof(unsigned long long)*MATRIX_SIZE);
    }
    for (int i=0;i<MATRIX_SIZE;i++)
    {
       for(int j=0;j<MATRIX_SIZE;j++)
       {
           unsigned long long sum = 0;
           for(int k=0;k<MATRIX_SIZE;k++)
           {
              int item1 = matrix1[i][k];
              int item2 = matrix2[k][j];
              unsigned long long mul = item1*item2;
              sum += matrix1[i][k]*matrix2[k][j];
          }
          result[i][j] = sum;
      }
    }
    return result;
}

int main()
{
    char* fileName1 = "sample1.txt";
    char* fileName2 = "sample2.txt";

    int** matrix1;
    int** matrix2;
    unsigned long long** result;
    unsigned long long sum = 0;
    matrix1 = readMatrix(fileName1);
    matrix2 = readMatrix(fileName2);
    result = matrixMultiply(matrix1, matrix2);

    for(int k = 0;k < MATRIX_SIZE; k++)
    {
        for(int j = 0;j < MATRIX_SIZE; j++)
        {
            sum += result[k][j];
        }
    }

    free(matrix1);
    free(matrix2);
    free(result);

    printf("sum = %llu\n", sum);

    return 0;
}
