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

typedef struct {
    sMatriz *matriz1, *matriz2, *resposta;
    int id, bloco;
} t_Args;

sMatriz * leMatriz (char * arquivo) {
    sMatriz *matriz;
    matriz = (sMatriz*) malloc(sizeof(sMatriz));
    FILE * descritorArquivo;

    descritorArquivo = fopen(arquivo, "rb");

    fread(&matriz->linhas, sizeof(int), 1, descritorArquivo);
    fread(&matriz->colunas, sizeof(int), 1, descritorArquivo);
    matriz->tamanho = matriz->linhas * matriz->colunas;    

    matriz->dados = (float *) malloc(sizeof(float) * matriz->tamanho);
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

sMatriz * multiplicaSequencial (sMatriz * matriz1, sMatriz * matriz2) {
    int tamanhoMultiplicacao;
    sMatriz * resposta;
    resposta = (sMatriz*) malloc(sizeof(sMatriz));

    if(matriz1->colunas != matriz2->linhas){
        printf("Essas matrizes não podem ser multiplicadas por inconsistências de tamanho.\n");
        return NULL;
    }

    resposta->linhas = matriz1->linhas;
    resposta->colunas = matriz2->colunas;
    tamanhoMultiplicacao = matriz1->colunas;
    resposta->tamanho = matriz1->linhas * matriz2->colunas;

    resposta->dados = (float *) malloc(sizeof(float) * resposta->tamanho);

    for(int linha = 0; linha < resposta->linhas; linha++){
        for(int coluna = 0; coluna < resposta->colunas; coluna++){
            float valor = 0;
            for(int ponteiro = 0; ponteiro < tamanhoMultiplicacao; ponteiro++){
                valor += matriz1->dados[linha * tamanhoMultiplicacao + ponteiro] * matriz2->dados[coluna + tamanhoMultiplicacao * ponteiro];
            }
            resposta->dados[linha * tamanhoMultiplicacao + coluna] = valor;
        }
    }

    return resposta;
}

void * tarefa(void * arg){
    t_Args *args = (t_Args *) arg;

    for(int i = args->id * args->bloco; i < args->id * args->bloco + args->bloco; i++){
        int linha, coluna, tamanhoMultiplicacao;
        float valor = 0;
        
        linha = i / args->matriz2->colunas;
        coluna = i % args->matriz2->colunas;
        tamanhoMultiplicacao = args->matriz1->colunas;

        for(int ponteiro = 0; ponteiro < tamanhoMultiplicacao; ponteiro++){
            valor += args->matriz1->dados[linha * tamanhoMultiplicacao + ponteiro] * args->matriz2->dados[coluna + tamanhoMultiplicacao * ponteiro];
        }

        args->resposta->dados[linha * tamanhoMultiplicacao + coluna] = valor;
    }

    pthread_exit(NULL);
}

sMatriz * multiplicaConcorrente(sMatriz * matriz1, sMatriz * matriz2, int numeroThreads){
    pthread_t threads[numeroThreads]; 
    t_Args *args; 
    sMatriz * resposta;
    resposta = (sMatriz*) malloc(sizeof(sMatriz));
    int tamanhoBloco;

    tamanhoBloco = matriz1->linhas * matriz2->colunas / numeroThreads;

    if(matriz1->colunas != matriz2->linhas){
        printf("Essas matrizes não podem ser multiplicadas por inconsistências de tamanho.\n");
        return NULL;
    }

    resposta->linhas = matriz1->linhas;
    resposta->colunas = matriz2->colunas;
    resposta->tamanho = matriz1->linhas * matriz2->colunas;

    resposta->dados = (float *) malloc(sizeof(float) * resposta->tamanho);

    for(int i = 0; i < numeroThreads; i++){
        args = malloc(sizeof(t_Args));
        args->id = i;
        args->bloco = tamanhoBloco;
        args->matriz1 = matriz1;
        args->matriz2 = matriz2;
        args->resposta = resposta;
        pthread_create(&threads[i], NULL, tarefa, (void *) args);
    }

    if(matriz1->linhas * matriz2->colunas % numeroThreads){
        // printf("Divisão inexata, workload na main.\n");
        for(int i = (matriz1->linhas * matriz2->colunas / numeroThreads) * numeroThreads; i < matriz1->linhas * matriz2->colunas; i++){
            int linha, coluna, tamanhoMultiplicacao;
            float valor = 0;
            
            linha = i / matriz2->colunas;
            coluna = i % matriz2->colunas;
            tamanhoMultiplicacao = matriz1->colunas;

            for(int ponteiro = 0; ponteiro < tamanhoMultiplicacao; ponteiro++){
                valor += matriz1->dados[linha * tamanhoMultiplicacao + ponteiro] * matriz2->dados[coluna + tamanhoMultiplicacao * ponteiro];
            }
            
            resposta->dados[linha * tamanhoMultiplicacao + coluna] = valor;
        } 
    }

    for(int i = 0; i < numeroThreads; i++){
        pthread_join(threads[i], NULL);
    }

    return resposta;
}

int main(int argc, char*argv[]) {
    // timer1

    sMatriz * matriz1;
    sMatriz * matriz2;
    
    FILE * descritorArquivo;
    descritorArquivo = fopen(argv[4], "wb");

    if(argc != 5){
        printf("Você deve passar o arquivo com as matrizes, o número de threads e o arquivo de saída. A execução foi cancelada.\n");
        return 1;
    }

    matriz1 = leMatriz(argv[1]);
    matriz2 = leMatriz(argv[2]);

    sMatriz * concorrente;
    // timer2
    concorrente = multiplicaConcorrente(matriz1, matriz2, atoi(argv[3]));
    // timer3
    printMatriz(concorrente);

    fwrite(&concorrente->linhas, sizeof(int), 1, descritorArquivo);
    fwrite(&concorrente->colunas, sizeof(int), 1, descritorArquivo);
    fwrite(concorrente->dados, sizeof(float), concorrente->tamanho, descritorArquivo);
    
    free(concorrente);
    // timer 4

    // sMatriz * sequencial;
    // sequencial = multiplicaSequencial(matriz1, matriz2);
    // printMatriz(sequencial);

    // fwrite(&sequencial->linhas, sizeof(int), 1, descritorArquivo);
    // fwrite(&sequencial->colunas, sizeof(int), 1, descritorArquivo);
    // fwrite(sequencial->dados, sizeof(float), sequencial->tamanho, descritorArquivo);

    // free(sequencial);

    free(matriz1);
    free(matriz2);
    return 0;
}