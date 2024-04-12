// HUGO FOLLONI GUARILHA - 121085854

// Projetar e implementar uma solução concorrente para o problema de multiplicação de matrizes, coletar informações sobre o seu tempo de execução, e calcular o ganho de desempenho obtido.

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include <pthread.h>

typedef struct {
    float *dados;
    int linhas, colunas, tamanho;
} sMatriz;


sMatriz * leMatriz (char * arquivo) {
    sMatriz *matriz;
    matriz = (sMatriz*) malloc(sizeof(sMatriz));
    if(matriz == NULL) {
        fprintf(stderr, "ERRO--malloc\n");
        return NULL;
    }
    FILE * descritorArquivo;

    descritorArquivo = fopen(arquivo, "rb");

    fread(&matriz->linhas, sizeof(int), 1, descritorArquivo);
    fread(&matriz->colunas, sizeof(int), 1, descritorArquivo);
    matriz->tamanho = matriz->linhas * matriz->colunas;    

    matriz->dados = (float *) malloc(sizeof(float) * matriz->tamanho);
    if(matriz->dados == NULL) {
        fprintf(stderr, "ERRO--malloc\n");
        return NULL;
    }
    fread(matriz->dados, sizeof(float), matriz->tamanho, descritorArquivo);

    return matriz;
}

void printMatriz(sMatriz * matriz){
    for(int i=0; i<matriz->linhas; i++) { 
        for(int j=0; j<matriz->colunas; j++)
            fprintf(stdout, "%.6f ", matriz->dados[i*matriz->colunas+j]);
        fprintf(stdout, "\n");
    }
}



int main(int argc, char*argv[]) {
    sMatriz * matriz;

    matriz = leMatriz(argv[1]);
    printMatriz(matriz);

    free(matriz);
    return 0;
}