#include<stdio.h>
#include<stdlib.h>
#include<time.h>

// #define PRINT

int main(int argc, char*argv[]) {
   float *matriz; 

   int linhas, colunas, tam; 
   FILE * descritorArquivo;
   size_t ret;
   
   if(argc < 4) {
      printf("Digite: %s <linhas> <colunas> <arquivo saida>\n", argv[0]);
      return 1;
   }

   linhas = atoi(argv[1]); 
   colunas = atoi(argv[2]);
   tam = linhas * colunas;

   matriz = (float*) malloc(sizeof(float) * tam);
   if(matriz == NULL) {
        fprintf(stderr, "ERRO--malloc\n");
        return 1;
    }

   srand(time(NULL));
   for(long int i=0; i<tam; i++) {
      *(matriz + i) = (rand() % 1000) * 0.3;
   }

   descritorArquivo = fopen(argv[3], "wb");

   ret = fwrite(&linhas, sizeof(int), 1, descritorArquivo);
   ret = fwrite(&colunas, sizeof(int), 1, descritorArquivo);
   ret = fwrite(matriz, sizeof(float), tam, descritorArquivo);

#ifdef PRINT
   for(int i=0; i<linhas; i++) {
      for(int j=0; j<colunas; j++){
         printf("%.6f ", matriz[i * colunas + j]);
      }
      printf("\n");
   }
#endif

   fclose(descritorArquivo);
   free(matriz);
   return 0;
}