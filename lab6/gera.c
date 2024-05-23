#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Você deve passar como argumento: <nome_do_arquivo> <N>\n");
        return 1;
    }

    char *nomeArquivo = argv[1];
    FILE *arquivo = fopen(nomeArquivo, "wb");
    if(!arquivo){printf("Não foi possível abrir o arquivo."); return 1;}
    int N = atoi(argv[2]);

    if (N <= 0) {
        printf("O número deve ser maior que 0.\n");
        return 1;
    }

   fwrite(&N, sizeof(int), 1, arquivo);

    srand(time(NULL));
    for(int i = 0; i < N; i++) {
        int valor = rand() % 10000;  
        fwrite(&valor, sizeof(int), 1, arquivo);
    }    

    fclose(arquivo);

    return 0;
}