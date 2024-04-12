// HUGO FOLLONI GUARILHA - 121085854

// Projetar, implementar e avaliar uma versao concorrente para o problema de somar todos os elementos de um vetor de numeros reais.

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define TEST

typedef struct {
    int idThread, tamBloco;
    float *vetor;
} sArgs;

typedef struct {
    float valor;
} sResponse;

void * soma (void * arg) {
    sArgs *args = (sArgs *) arg;

    sResponse *somaLocal;
    somaLocal = (sResponse *) malloc(sizeof(sResponse));

    somaLocal->valor = 0;

    for(int i = args->idThread * args->tamBloco; i < args->idThread * args->tamBloco + args->tamBloco; i++){
        somaLocal->valor += args->vetor[i];
    }

    free(arg);
    pthread_exit((void *) somaLocal);
}


int main(int argc, char *argv[]){
    float *vetor;

    if(argc < 2) {
        printf("Você não passou os argumentos de forma correta. Você deve dizer o número de threads que esse programa usará.");
        return 0;
    }

    float somaGeral = 0;
    sResponse *somaLocal;
    long int N, M;

    M = atoi(argv[1]);

    pthread_t *threads;
    threads = (pthread_t *) malloc(sizeof(pthread_t) * M);
    sArgs *args;

    scanf("%ld", &N);

    vetor = (float *) malloc(sizeof(float) * N);

    for(int i = 0; i < N; i++){
        scanf("%f", &vetor[i]);    
    }

    for(int i = 0; i < M; i++){
        args = malloc(sizeof(sArgs));
        args->idThread = i;
        args->tamBloco = N / M;
        args->vetor = vetor;
        pthread_create(&threads[i], NULL, soma, (void *) args);
    }

    if(N % M){
        for(int i = (N / M) * M; i < N; i++){
            somaGeral += vetor[i];
        } 
    }

    for(int i = 0; i < M; i++){
        pthread_join(threads[i], (void **) &somaLocal);
        somaGeral += somaLocal->valor;
    }

    printf("%f\n", somaGeral);

    #ifdef TEST
        float somaReal = 0;
        scanf("%f", &somaReal);
        if(abs(somaGeral - somaReal) < 0.001){
            printf("Resultado correto.\n");
        }        
        else{
            printf("Resultado errado.\n");
        }
    #endif

    free(vetor);
    free(threads);

    return 0;
}