#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define MATRIX_SIZE 4000
int num_thrd;
unsigned long long** A;
unsigned long long** B;
unsigned long long** C;

// matrix multiply function with argument slice
void* multiply(void* slice)
{
    int s = (int)slice;
    int from = (s * MATRIX_SIZE) / num_thrd;
    int to = ((s + 1) * MATRIX_SIZE) / num_thrd;
    int i,j,k;
    for(i = from; i < to; i++){
        for(j = 0; j < MATRIX_SIZE; j++)
        {
            C[i][j] = 0;
            for(k = 0; k < MATRIX_SIZE; k++){
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return 0;
}

// read matrix from matrix file
unsigned long long** readMatrix(char* fileName)
{
    FILE *fp;
    unsigned long long** matrix;

    matrix = malloc(sizeof(unsigned long long*) * MATRIX_SIZE);
    int i = 0;
    int j = 0;
    int k = 0;
    for(i = 0; i < MATRIX_SIZE; i++)
    {
        matrix[i] = malloc(sizeof(unsigned long long) * MATRIX_SIZE);
        if(matrix[i] == NULL)
        {
            fprintf(stderr, "out of memory\n");
            exit(0);
        }
    }

    fp = fopen(fileName,"r");
    for(k = 0; k < MATRIX_SIZE; k++)
    {
        for(j = 0; j < MATRIX_SIZE; j++)
        {
          unsigned long long test;
          fscanf(fp, "%llu", &test);
          matrix[k][j] = test;
        }
    }
    fclose(fp);
    return matrix;
}

int main(int argc, char* argv[])
{
    pthread_t* thread;
    unsigned long long sum = 0;
    if(argc != 2)
    {
      printf("Usage: %s number_of_threads\n", argv[0]);
      exit(1);
    }

    //read matrix A and B
    char* fileName1 = "sample1.txt";
    char* fileName2 = "sample2.txt";

    A = readMatrix(fileName1);
    B = readMatrix(fileName2);

    // allocate matrix C use for multiplication result
    C = malloc(sizeof(unsigned long long*) * MATRIX_SIZE);
    int i = 0;
    int j = 0;
    for(i = 0; i < MATRIX_SIZE; i++)
    {
        C[i] = malloc(sizeof(unsigned long long) * MATRIX_SIZE);
        if(C[i] == NULL)
        {
            fprintf(stderr, "out of memory\n");
            exit(1);
        }
    }

    // make threads array
    num_thrd = atoi(argv[1]);
    thread = (pthread_t*)malloc(num_thrd * sizeof(pthread_t));

    for(i = 1; i < num_thrd; i++)
    {
        // create threads for multiply function and give argument i for slice
        if(pthread_create(&thread[i], NULL, multiply, (void*)i) != 0 )
        {
            perror("Can't create thread");
            free(thread);
            exit(1);
        }
    }

    // main thread is slice 0
    multiply(0);

    // main thread waits for other threads
    for(i = 1; i < num_thrd; i++)
        pthread_join(thread[i], NULL);

    for(int j = 0; j < MATRIX_SIZE; j++) {
        for(int k = 0; k < MATRIX_SIZE; k++)
            sum += C[j][k];
    }

    for(i = 0; i < MATRIX_SIZE; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);
    free(thread);

    printf("sum = %llu\n", sum);

    return 0;
}
