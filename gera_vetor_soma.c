#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1000 // Valor máximo para cada elemento do vetor

int main(int argc, char *argv[]) {
    int N;
    float soma = 0;

    if(argc < 2) {
       printf("Use: %s <numero de elementos (maior que 0)> \n", argv[0]);
       return 1;
    }

    srand((unsigned)time(NULL));
    N = atoi(argv[1]);

    printf("%d\n", N);

    for(int i = 0; i < N; i++){
        float random = (rand() % MAX + 1) / ((float) (rand() % 5) + 1);
        soma += random;
        printf("%f ", random);
    }

    printf("\n%f", soma);

    return 0;
}