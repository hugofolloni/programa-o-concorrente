#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include "timer.h"
#include<math.h>

#define NTHREADS  4

long long int n;
long int quantidade;
long int posicao = 0;

int ehPrimo(long long int n) {
int i;
    if (n<=1) return 0;
    if (n==2) return 1;
    if (n%2==0) return 0;
    for (i=3; i<sqrt(n)+1; i+=2)
    if(n%i==0) return 0;
        return 1;
}
//funcao executada pelas threads
void *ExecutaTarefa (void *threadid) {
    while(n > posicao){
        int valor = posicao;
        posicao++;
        int resultado = ehPrimo(valor);
        quantidade += resultado;
        // printf("Thread %d pegou numero %d, achou %d\n", tid, valor, resultado);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    double inicio, fim;
    pthread_t tid[NTHREADS];
    int t, id[NTHREADS];

    n = atoi(argv[1]);

        GET_TIME(inicio);
    for(t=0; t<NTHREADS; t++) {
        id[t]=t;
        if (pthread_create(&tid[t], NULL, ExecutaTarefa, (void *) &id[t])) {
        printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }

    for (t=0; t<NTHREADS; t++) {
        if (pthread_join(tid[t], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1); 
        } 
    } 

    printf("Quantidade de primos = %ld\n", quantidade);
    GET_TIME(fim);
    printf("%lf\n", fim - inicio);
}