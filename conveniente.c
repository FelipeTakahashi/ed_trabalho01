#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXCHAR 256
#define MAXSIZE 11777
#define N_DIM 2

typedef struct _City{
    char codigo_ibge[20];
    char nome[50];
    double latitude;
    double longitude;
    int capital;
    int codigo_uf;
    int siafi_id;
    int ddd;
    char fuso_horario[50];
}typeCity;

typedef struct _Hash{
    typeCity** table;
    int maxSize;
    int currentSize;
}typeHash;

typedef struct _heap{
    typeCity* city;
    double distance;
}typeHeap;

typedef struct _maxHeap{
    typeHeap* heap;
    int maxSize;
    int size;
}maxHeap;

typedef struct _node{
	typeCity* city;
	struct _node* left;
	struct _node* right;
}typeNode;

typeCity* cityAlloc(char* codigo_ibge, char* nome, double latitude, double longitude, int capital, int codigo_uf, int siafi_id, int ddd, char* fuso_horario) {
    typeCity* city = malloc(sizeof(typeCity));
    strcpy(city->codigo_ibge, codigo_ibge);
    strcpy(city->nome, nome);
    city->latitude = latitude;
    city->longitude = longitude;
    city->capital = capital;
    city->codigo_uf = codigo_uf;
    city->siafi_id = siafi_id;
    city->ddd = ddd;
    strcpy(city->fuso_horario, fuso_horario);

    return city;
}

void printCity(typeCity* city) {
    printf("\nInformações da cidade:\n");
    printf("Nome: %s\n", city->nome);
    printf("Código IBGE: %s\n", city->codigo_ibge);
    printf("Latitude: %lf\n", city->latitude);
    printf("Longitude: %lf\n", city->longitude);
    printf("Capital: %d\n", city->capital);
    printf("Código UF: %d\n", city->codigo_uf);
    printf("Siafi ID: %d\n", city->siafi_id);
    printf("DDD: %d\n", city->ddd);
    printf("Fuso Horário: %s\n", city->fuso_horario);
}

void printRow(typeCity* city) {
    printf("{%s, %s, %.4lf, %.4lf, %d, %d, %d, %d, %s}\n", city->codigo_ibge, city->nome, city->latitude, city->longitude, city->capital, city->codigo_uf, city->siafi_id, city->ddd, city->fuso_horario);
}

void printRowWithDistance(typeCity* city, double distance) {
    printf("{%s, %s, %.4lf, %.4lf, %d, %d, %d, %d, %s, %.6lf}\n", city->codigo_ibge, city->nome, city->latitude, city->longitude, city->capital, city->codigo_uf, city->siafi_id, city->ddd, city->fuso_horario, sqrt(distance));
}

// --- HASH FUNCTIONS --- //

int hashFunction1(char *key) {
    int sum = 0;
    int length = strlen(key);
    for(int i = 0; i < length; i++) {
        sum += key[i];
    }
    return sum % MAXSIZE;
}

int hashFunction2(char *key) {
    int sum = 0;
    int length = 0;
    for(int i = 0; i < length; i++) {
        sum += key[i];
    }

    return 1 + (sum % (MAXSIZE-1)); // incremento primo
}

int hashInit(typeHash* hash, int buckets) {
    int limit = buckets+1;

    hash->table = calloc(sizeof(typeCity), limit);

    if(hash->table == NULL) {
        printf("Não foi possível alocar a Hash.\n");
        return EXIT_FAILURE;
    }

    hash->maxSize = limit;
    hash->currentSize = 0;

    return EXIT_SUCCESS;
}

void hashInsertByCode(typeHash* hash, typeCity* bucket) {
    int index = hashFunction1(bucket->codigo_ibge) % hash->maxSize;

    if(hash->maxSize == hash->currentSize + 1) {
        printf("Impossível Alocar\n");
        return;
    } else {
        int occurrences = 1;
        while(hash->table[index] != NULL) {
            index = index + (occurrences++ * hashFunction2(bucket->codigo_ibge)) % hash->maxSize;
        }

        hash->table[index] = bucket;
        hash->currentSize++;
    }
    return;
}

void hashInsertByName(typeHash* hash, typeCity* bucket) {
    int index = hashFunction1(bucket->nome) % hash->maxSize;

    if(hash->maxSize == hash->currentSize + 1) {
        printf("Impossível Alocar\n");
        return;
    } else {
        int occurrences = 1;
        while(hash->table[index] != NULL) {
            index = index + (occurrences++ * hashFunction2(bucket->nome)) % hash->maxSize;
        }

        hash->table[index] = bucket;
        hash->currentSize++;
    }
    return;
}

typeCity* hashSearchByCode(typeHash* hash, char* key) {
    int index = hashFunction1(key) % hash->maxSize;
    typeCity* ret = NULL;

    int occurrences = 1;
    while(hash->table[index] != NULL && ret == NULL) {
        if(strcmp(hash->table[index]->codigo_ibge, key) == 0) {
            ret = hash->table[index];
        } else {
            index = index + (occurrences++ * hashFunction2(key)) % hash->maxSize;
        }
    }

    if(ret == NULL) {
        printf("Cidade não encontrada.\n");
    }
    return ret;
}

char* hashSearchByName(typeHash* hash, char* key) {
    int candidatesMax = 5;
    int candidatesSize = 0;

    typeCity** candidates = malloc(candidatesMax * sizeof(typeCity*));
    typeCity* ret = NULL;

    int occurrences = 1;
    int index = hashFunction1(key) % hash->maxSize;

    while(hash->table[index] != NULL) {
        if(strcmp(hash->table[index]->nome, key) == 0) {

            if(candidatesMax <= candidatesSize) {
                candidates = realloc(candidates, candidatesMax * 2 * sizeof(typeCity*));
                candidatesMax *= 2;
            }

            ret = hash->table[index];
            candidates[candidatesSize++] = hash->table[index];

            index = index + (occurrences++ * hashFunction2(key)) % hash->maxSize;
        } else {
            index = index + (occurrences++ * hashFunction2(key)) % hash->maxSize;
        }
    }

    if(ret == NULL) {
        printf("Cidade não encontrada.\n");
        return NULL;
    }

    if(candidatesSize > 1) {
        printf("\nMais de uma cidade com o nome \"%s\" foi encontrada.\n", ret->nome);
        printf("Digite a opção de seu destino: \n");

        for(int i = 0; i < candidatesSize; i++) {
            printf("%d. ", i+1);
            printRow(candidates[i]);
        }

        int option;
        scanf("%d", &option);

        if(option > candidatesSize || option < 0) return NULL;

        return candidates[option-1]->codigo_ibge;
    }

    return ret->codigo_ibge;
}

void hashDestroy(typeHash* hash) {
    for(int i = 0; i < hash->maxSize; i++) {
        if(hash->table[i] != 0) {
            free(hash->table[i]);
        }
    }

    free(hash->table);
    return;
}

double distance(typeCity a, typeCity b) {
    double x = a.latitude - b.latitude;
    double y = a.longitude - b.longitude;
    return x*x + y*y;
}

typeCity* cityAllocSimplified(char* name, double latitude, double longitude) {
	typeCity* city = malloc(sizeof(typeCity));
	strcpy(city->nome, name);
	city->latitude = latitude;
	city->longitude = longitude;
	return city;
}

maxHeap* createMaxHeap(int maxSize){
    maxHeap* heap = malloc(sizeof(maxHeap));
    heap->heap = malloc(sizeof(typeHeap) * maxSize);
    heap->maxSize = maxSize;
    heap->size = 0;

    return heap;
}

void swap(typeHeap* a, typeHeap* b){
    typeHeap aux = *a;
    *a = *b;
    *b = aux;
}

void heapifyDown(maxHeap* heap, int i) {
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int max = i;

    if(left < heap->size && heap->heap[left].distance > heap->heap[max].distance){
        max = left;
    }

    if(right < heap->size && heap->heap[right].distance > heap->heap[max].distance){
        max = right;
    }

    if(max != i){
        swap(&heap->heap[i], &heap->heap[max]);
        heapifyDown(heap, max);
    }
}

void heapifyUp(maxHeap* heap, int i){
    int max = (i-1)/2;
    if(heap->heap[max].distance < heap->heap[i].distance){
        swap(&heap->heap[max], &heap->heap[i]);
        heapifyUp(heap, max);
    }
}

void heapShift(maxHeap* heap, typeCity* city, double distance){
    if(heap->size == heap->maxSize){
        if(distance < heap->heap[0].distance){
            heap->heap[0].city = city;
            heap->heap[0].distance = distance;
            heapifyDown(heap, 0);
        }
    } else {
        heap->heap[heap->size].city = city;
        heap->heap[heap->size].distance = distance;
        heapifyUp(heap, heap->size);
        heap->size++;
    }
}

void heapSort(maxHeap* heap){
    for(int i = heap->size-1; i > 0; i--){
        swap(&heap->heap[0], &heap->heap[i]);
        heap->size--;
        heapifyDown(heap, 0);
    }
}

void freeHeap(maxHeap* heap){
    free(heap->heap);
    free(heap);
}

typeNode* createNode(typeCity* city) {
	typeNode* node = malloc(sizeof(typeNode));

	node->city = city;
	node->left = NULL;
	node->right = NULL;

	return node;
}

void kdInsert(typeNode** node, typeCity* city, int dimension) {
	if(*node == NULL) {
	   *node = createNode(city);
		return;
	}

	if(dimension == 0) {
	   if(city->latitude < (*node)->city->latitude) {
			kdInsert(&((*node)->left), city, (dimension+1) % N_DIM);
	   } else {
			kdInsert(&((*node)->right), city, (dimension+1) % N_DIM);
	   }
	} else {
	   if(city->longitude < (*node)->city->longitude) {
			kdInsert(&((*node)->left), city, (dimension+1) % N_DIM);
	   } else {
			kdInsert(&((*node)->right), city, (dimension+1) % N_DIM);
	   }
	}
}

int checkInHeap(typeCity* city, maxHeap* heap) {
    for(int i = 0;  i < heap->size; i++) {
        if(strcmp(city->codigo_ibge, heap->heap[i].city->codigo_ibge) == 0) {
            return 1;
        }
    }
    return 0;
}

void knn(typeNode* node, typeCity* city, maxHeap* heap, int k, int dimension) {
    if(node == NULL) {
        return;
    }

    double dist = distance(*city, *(node->city));
    double currentMax = 0;

    if(heap->size > 0) currentMax = heap->heap[0].distance;

    if(strcmp(node->city->codigo_ibge, city->codigo_ibge) != 0 && !checkInHeap(node->city, heap)) {
        heapShift(heap, node->city, dist);
    }

    double reference;
    double current;

    if(dimension == 0) {
        reference = city->latitude;
        current = node->city->latitude;
    } else {
        reference = city->longitude;
        current = node->city->longitude;
    }

    double planeDistance;
    planeDistance = reference - current;
    if(planeDistance < 0) planeDistance = -planeDistance;

    if(reference < current) {
        knn(node->left, city, heap, k, (dimension+1) % N_DIM);
        if(heap->size < k || planeDistance <= sqrt(currentMax)) {
            knn(node->right, city, heap, k, (dimension+1) % N_DIM);
        }
    } else {
        knn(node->right, city, heap, k, (dimension+1) % N_DIM);
        if(heap->size < k || planeDistance <= sqrt(currentMax)) {
            knn(node->left, city, heap, k, (dimension+1) % N_DIM);
        }
    }
}

void preorder(typeNode* tree) {
	if(tree != NULL) {
		printf("%s\n", tree->city->nome);
		preorder(tree->left);
		preorder(tree->right);
	}
}

void kdDestroy(typeNode* tree) {
    if(tree != NULL) {
        kdDestroy(tree->left);
        kdDestroy(tree->right);
        free(tree);
    }
}

// --- PARSER FUNCTIONS --- //


void removeQuotation(char* value) {
    if(value == NULL) return;

    int length = strlen(value);

    if(value[0] == '\"' && value[length-1] == '\"') {
        memmove(value, value + 1, length - 2);
        value[length - 2] = '\0';
    }
}

typeCity* copyCity(typeCity* city) {
    typeCity* newCity = malloc(sizeof(typeCity));

    strcpy(newCity->codigo_ibge, city->codigo_ibge);
    strcpy(newCity->nome, city->nome);
    newCity->latitude = city->latitude;
    newCity->longitude = city->longitude;
    newCity->capital = city->capital;
    newCity->codigo_uf = city->codigo_uf;
    newCity->siafi_id = city->siafi_id;
    newCity->ddd = city->ddd;
    strcpy(newCity->fuso_horario, city->fuso_horario);

    return newCity;
}

void parse(typeHash* hashC, typeNode** tree, typeHash* hashN) {
    FILE *readJSON = fopen("../municipios.json", "r");
    if (!readJSON) {
        return;
    }

    char line[100];
    int dataCounter = 0;

    typeCity* city = malloc(sizeof(typeCity));
    if (!city) {
        fclose(readJSON);
        return;
    }

    while (fgets(line, sizeof(line), readJSON)) {
        char *token = strtok(line, ":\t\n\r");
        while (token != NULL) {
            token = strtok(NULL, ":\t\n,{}]");

            if (token == NULL) {
                break;
            }

            char* blankspacePointer = strchr(token, ' ');

            if (blankspacePointer != NULL) {
                dataCounter++;
                char* value = blankspacePointer + 1;

                switch (dataCounter) {
                case 1:
                    strcpy(city->codigo_ibge, value);
                    break;
                case 2:
                    removeQuotation(value);
                    strcpy(city->nome, value);
                    break;
                case 3:
                    city->latitude = atof(token);
                    break;
                case 4:
                    city->longitude = atof(token);
                    break;
                case 5:
                    city->capital = atoi(token);
                    break;
                case 6:
                    city->codigo_uf = atoi(token);
                    break;
                case 7:
                    city->siafi_id = atoi(token);
                    break;
                case 8:
                    city->ddd = atoi(token);
                    break;
                case 9:
                    removeQuotation(value);
                    strcpy(city->fuso_horario, value);
                    dataCounter = 0;

                    hashInsertByCode(hashC, copyCity(city));
                    hashInsertByName(hashN, copyCity(city));
                    kdInsert(tree, copyCity(city), 0);
                    break;
                }
            }
        }
    }
    free(city);

    fclose(readJSON);
}

void menu() {
    printf("\n-------------------------------\n");
    printf("\nSelecione uma operação: \n");
    printf("1 : Buscar cidade pelo código IBGE.\n");
    printf("2 : Buscar k vizinhos pelo código IBGE.\n");
    printf("3 : Buscar k vizinhos pelo nome.\n");
    printf("4 : Finalizar\n");
    printf("\n-------------------------------\n");
}

void taskOne(typeHash* hash) {
    char * lookFor = malloc(sizeof(char) * 10);
    printf("Digite o código IBGE: \n");
    scanf("%s", lookFor);

    typeCity* searched = hashSearchByCode(hash, lookFor);

    if(searched == NULL) {
        printf("Operação finalizada.\n");
        free(lookFor);
        return;
    }

    printf("\n1. ");
    printRow(searched);

    free(lookFor);
}

void taskTwo(typeHash* hash, typeNode* kdtree) {
    char * lookFor = malloc(sizeof(char) * 10);
    int k;

    printf("Digite o código IBGE: \n");
    scanf("%s", lookFor);

    printf("Digite quantos vizinhos procurar: \n");
    scanf("%d", &k);

    if(k > 5569 || k < 1) {
        printf("Valor inválido.\n");
        free(lookFor);
        return;
    }

    typeCity* searched = hashSearchByCode(hash, lookFor);

    if(searched == NULL) {
        printf("Operação finalizada.\n");
        free(lookFor);
        return;
    }

    maxHeap* heap = createMaxHeap(k);
    knn(kdtree, searched, &(*heap), k, 0);
    heapSort(heap);

    printf("\nOs %d vizinhos mais próximos de {%s, %s} são: \n", k, searched->nome, searched->codigo_ibge);

    for(int i = 0; i < k; i++) {
        if(heap->heap[i].city == NULL) {
            printf("Posição na heap inválida.\n");
        }
        printf("%d. %s\n", i+1, heap->heap[i].city->codigo_ibge);
    }

    freeHeap(heap);
    free(lookFor);
}

void taskThree(typeHash* hashC, typeHash* hashN, typeNode* kdtree) {
    char lookFor[50];

    printf("Digite o nome da cidade: \n");
    scanf(" %[^\n]s", lookFor);

    char* searchedCode = hashSearchByName(hashN, &(*lookFor));

    if(searchedCode == NULL) {
        printf("Operação finalizada.\n");
        return;
    }

    typeCity* searchedCity = hashSearchByCode(hashC, searchedCode);

    if(searchedCity == NULL) {
        printf("Operação finalizada.\n");
        return;
    }

    int k;
    printf("Digite quantos vizinhos procurar: \n");
    scanf("%d", &k);

    if(k > 5569 || k < 1) {
        printf("Valor inválido.\n");
        return;
    }

    maxHeap* heap = createMaxHeap(k);
    knn(kdtree, searchedCity, &(*heap), k, 0);
    heapSort(heap);

    printf("\nOs %d vizinhos mais próximos de %s são: \n", k, searchedCity->nome);

    for(int i = 0; i < k; i++) {
        if(heap->heap[i].city == NULL) {
            printf("Posição na heap inválida.\n");
        }
        printf("%d. ", i+1);
        printRowWithDistance(heap->heap[i].city, heap->heap[i].distance);
        //printf("%d. %s : %s\n", i+1, heap->heap[i].city->nome, heap->heap[i].city->codigo_ibge);
    }
    freeHeap(heap);
}

void exec(typeHash* hashC, typeNode* kdtree, typeHash* hashN) {
    int op = 0;
    do {
        menu();
        printf("\nDigite a operação a ser realizada\n");
        scanf("%d", &op);

        int k;

        switch (op) {
            case 1:
                taskOne(hashC);
                break;
            case 2:
                taskTwo(hashC, kdtree);
                break;
            case 3:
                taskThree(hashC, hashN, kdtree);
                break;
            case 4:
                op = 4;
                break;
            default:
                op = 4;
                printf("Operação inválida.\n");
                break;
        }
    } while(op != 4);

    printf("Operação finalizada.\n");
}

int main() {
    typeHash* hashCode = malloc(sizeof(typeHash));
    typeHash* hashName = malloc(sizeof(typeHash));
    hashInit(hashCode, MAXSIZE);
    hashInit(hashName, MAXSIZE);

    typeNode* kdtree = NULL;

    parse(hashCode, &kdtree, hashName);

    printf("\n-------------------------------\n\n");
    printf("Estrutura de Dados - Trabalho 01\n");

    exec(hashCode, kdtree, hashName);

    hashDestroy(hashCode);
    hashDestroy(hashName);
    kdDestroy(kdtree);

    return EXIT_SUCCESS;
}
