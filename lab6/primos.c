#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <semaphore.h>

int * buffer; 
int M;
int prod_pointer = 0;
int consumer_pointer = 0;
int producao_terminada = 0;

sem_t sem_prod;
sem_t sem_cons;      
sem_t sem_full; 
sem_t sem_items; 
sem_t sem_primos;
sem_t sem_vencedor;

typedef struct {
    int id;
    int quantidade;
} Vencedora;

Vencedora vencedora;
int quantidade = 0;

typedef struct {
    int id;
} idThread;

int ehPrimo(long long int n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; i <= sqrt(n); i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

void *produtora(void *nome) {
    const char *nomeArquivo = (const char *) nome; 
    FILE *file = fopen(nomeArquivo, "rb");
    if (file == NULL) { printf("Erro ao abrir o arquivo.\n"); exit(1); }

    int num;

    while (fread(&num, sizeof(int), 1, file)) {
        // sem_wait(&sem_full); // Caso buffer esteja cheio, esperar

        sem_wait(&sem_prod);       // Seção crítica do index do produtor
        buffer[prod_pointer++] = num;
        sem_post(&sem_prod);    

        sem_post(&sem_items);  // Informa que pode ser pego um novo item
    }

    fclose(file);

    producao_terminada = 1;

    sem_post(&sem_items); // Libera a última thread (começou parada)

    pthread_exit(0);
}

void *consumidora(void *id) {
    idThread * thread = (idThread *) id;

    int local = 0;

    while (1) {
        sem_wait(&sem_items);
        
        sem_wait(&sem_cons); // Seção crítica do index do consumidor

        if (producao_terminada && prod_pointer == consumer_pointer) { // Se não terá novos itens e já lemos todos, break
            sem_post(&sem_cons);
            sem_post(&sem_items); // Libera a próxima thread, que checará que não existem novos itens
            break;
        }

        int num = buffer[consumer_pointer++];

        sem_post(&sem_cons); 

        // sem_post(&sem_full);

        if (ehPrimo(num)) {
            local++;
        }
    }

    sem_wait(&sem_primos); // Seção crítica da quantidade de primos
    quantidade = quantidade + local;
    sem_post(&sem_primos);

    sem_wait(&sem_vencedor); // Seção crítica do vencedor
    if(local > vencedora.quantidade){
       vencedora.quantidade = local;
       vencedora.id = thread->id;
    }
    sem_post(&sem_vencedor);

    pthread_exit(0);
}

int main(int argc, char *argv[]) {

    if (argc != 4) {
        printf("Você deve fornecer os itens da seguinte forma: $ %s <arquivo_de_entrada> <numero_de_threads> <tamanho_do_buffer>\n", argv[0]);
        return 1;
    }

    int consumidoras = atoi(argv[2]);
    M = atoi(argv[3]);

    buffer = (int *) malloc(M * sizeof(int));
    if (buffer == NULL) { printf("Erro ao criar o buffer"); return 1; }

    sem_init(&sem_full, 0, M);
    sem_init(&sem_items, 0, 0);
    sem_init(&sem_prod, 0, 1);
    sem_init(&sem_cons, 0, 1);
    sem_init(&sem_primos, 0, 1);
    sem_init(&sem_vencedor, 0, 1);

    pthread_t threadProdutora;
    pthread_t threadsConsumidoras[consumidoras];

    pthread_create(&threadProdutora, NULL, produtora, (void *) argv[1]);

    idThread *threadId;
    vencedora.quantidade = 0;

    for (int i = 0; i < consumidoras; i++) {
        threadId = malloc(sizeof(idThread));
        threadId->id = i;
        pthread_create(&threadsConsumidoras[i], NULL, consumidora, (void *) threadId);
    }

    pthread_join(threadProdutora, NULL);

    for (int i = 0; i < consumidoras; i++) {
        pthread_join(threadsConsumidoras[i], NULL);
    }

    printf("Primos: %d\n", quantidade);
    printf("Thread vencedora: %d\n", vencedora.id);

    sem_destroy(&sem_full);
    sem_destroy(&sem_items);
    sem_destroy(&sem_prod);
    sem_destroy(&sem_cons);
    sem_destroy(&sem_primos);
    sem_destroy(&sem_vencedor);
    free(buffer);

    return 0;
}