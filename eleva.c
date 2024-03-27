// HUGO FOLLONI GUARILHA - 121085854

// Escreva um programa de M threads, para elevar ao quadrado cada elemento de um vetor de N elementos do tipo inteiro.


#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

typedef struct {
   int idThread, nThreads, tamanhoVetor, *vetor;
} t_Args;

void inicializaVetor(int N, int *vetor) {
    printf("O vetor foi criado com os números: ");
    for(int i = 0; i < N; i++){
        vetor[i] = rand() % 10;
        printf("%d ", vetor[i]);
    }
    printf("\n");
}

void printaVetor(int N, int *vetor) {
    printf("O vetor atual é composto por: ");
    for(int i = 0; i < N; i++){
        printf("%d ", vetor[i]);
    }
    printf("\n");
}

void *elevaNumeros (void *arg){
    t_Args *args = (t_Args *) arg;

    int inicio = args->idThread * (args->tamanhoVetor / args->nThreads);
    int fim = inicio + (args->tamanhoVetor / args->nThreads);

    for(int i = inicio; i < fim; i++){
        args->vetor[i] *= args->vetor[i];   
    }

    if(args->idThread == args->nThreads - 1){
        int inicioExtra = inicio + (args->tamanhoVetor / args->nThreads);
        printf("%d \n", inicioExtra);
        for(int i = inicioExtra; i < args->tamanhoVetor; i++){
            args->vetor[i] *= args-> vetor[i];
        }
    }

    free(args);
    pthread_exit(NULL);
}

int main (int argc, char *argv[]) {

    int N, M, *vetor;

    if(argc != 3){
        printf("Você deve passar o tamanho do vetor e o número threads. A execução foi cancelada.");
        return 1;
    }

    N = atoi(argv[1]);
    M = atoi(argv[2]);
    vetor = (int *)malloc(N * sizeof(int));
    
    inicializaVetor(N, vetor);

    pthread_t threads[M]; 
    t_Args *args; 

    for(int i = 0; i < M; i++) {
        args = malloc(sizeof(t_Args));
        args->idThread = i; 
        args->nThreads = M; 
        args->tamanhoVetor = N;
        args->vetor = vetor;
        
        pthread_create(&threads[i], NULL, elevaNumeros, (void*) args);
    }

    for(int i = 0; i < M; i++){
        pthread_join(threads[i], NULL);
    }

    printaVetor(N, vetor);

    free(vetor);
    return 0;
}