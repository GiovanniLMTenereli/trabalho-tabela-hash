//Feito por Giovanni Lisboa de Miranda Tenereli
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TABLE_SIZE 100

typedef struct HashNode {
    char *nome;
    char *tel;
    struct HashNode *prox;
} HashNode;

typedef struct HashTable {
    HashNode *contatos[TABLE_SIZE];
} HashTable;

unsigned int hash(const char *nome) {
    unsigned int hash_value = 0;
    while (*nome) {
        hash_value = (hash_value << 5) + *nome++;
    }
    return hash_value % TABLE_SIZE;
}

HashTable *init_table() {
    HashTable *table = (HashTable *)malloc(sizeof(HashTable));
    for (int i = 0; i < TABLE_SIZE; i++) {
        table->contatos[i] = NULL;
    }
    return table;
}

void adicionarContato(HashTable *table, const char *nome, char *tel) {
    clock_t start, end;
    start = clock();

    unsigned int hash_index = hash(nome);
    HashNode *node = table->contatos[hash_index];

    while (node) {
        if (strcmp(node->nome, nome) == 0) {
            printf("Contato com nome '%s' já existe! Atualizando o telefone.\n", nome);
            free(node->tel);
            node->tel = strdup(tel);

            end = clock();
            printf("Tempo de inserção: %f ms\n", ((double)(end - start)) / CLOCKS_PER_SEC * 1000);
            return;
        }
        node = node->prox;
    }

    HashNode *new_node = (HashNode *)malloc(sizeof(HashNode));
    new_node->nome = strdup(nome);
    new_node->tel = strdup(tel);
    new_node->prox = table->contatos[hash_index];
    table->contatos[hash_index] = new_node;

    end = clock();
    printf("Contato '%s' adicionado com sucesso.\n", nome);
    printf("Tempo de inserção: %f ms\n", ((double)(end - start)) / CLOCKS_PER_SEC * 1000);
}

char *buscarContato(HashTable *table, const char *nome) {
    clock_t start, end;
    start = clock();

    unsigned int hash_index = hash(nome);
    HashNode *node = table->contatos[hash_index];
    while (node) {
        if (strcmp(node->nome, nome) == 0) {
            end = clock();
            printf("Tempo de busca: %f ms\n", ((double)(end - start)) / CLOCKS_PER_SEC * 1000);
            return node->tel;
        }
        node = node->prox;
    }

    end = clock();
    printf("Tempo de busca: %f ms\n", ((double)(end - start)) / CLOCKS_PER_SEC * 1000);
    return NULL;
}

void removerContato(HashTable *table, const char *nome) {
    unsigned int hash_index = hash(nome);
    HashNode *node = table->contatos[hash_index];
    HashNode *anterior = NULL;

    while (node != NULL) {
        if (strcmp(node->nome, nome) == 0) {
            if (anterior == NULL) {
                table->contatos[hash_index] = node->prox;
            } else {
                anterior->prox = node->prox;
            }
            free(node->nome);
            free(node->tel);
            free(node);
            printf("Contato '%s' removido com sucesso.\n", nome);
            return;
        }
        anterior = node;
        node = node->prox;
    }
    printf("Contato '%s' não encontrado.\n", nome);
}

void exibirContatos(HashTable *table) {
    int achados = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode *node = table->contatos[i];
        while (node) {
            achados++;
            printf("Nome: %s\nTelefone: %s\n", node->nome, node->tel);
            node = node->prox;
        }
    }
    if (achados == 0) {
        printf("A tabela está vazia!\n");
    }
}

int main() {
    int opcao;
    HashTable *hash_table = init_table();
    char nome[50];
    char telefone[20];

    do {
        printf("\nEscolha uma opção:\n");
        printf("1 - Adicionar contato\n");
        printf("2 - Buscar contato por nome\n");
        printf("3 - Remover contato\n");
        printf("4 - Exibir todos os contatos\n");
        printf("0 - Sair\n");
        printf("Digite uma opção: ");

        scanf("%d", &opcao);
        while (getchar() != '\n');

        switch (opcao) {
            case 1:
                printf("\nDigite o nome do contato: ");
                scanf("%49s", nome);
                printf("Digite o telefone do contato: ");
                scanf("%19s", telefone);
                adicionarContato(hash_table, nome, telefone);
                break;
            case 2:
                printf("\nDigite o nome do contato: ");
                scanf("%49s", nome);
                char *resultado = buscarContato(hash_table, nome);
                if (resultado) {
                    printf("Telefone de %s: %s\n", nome, resultado);
                } else {
                    printf("Contato '%s' não encontrado.\n", nome);
                }
                break;
            case 3:
                printf("\nDigite o nome do contato a ser removido: ");
                scanf("%49s", nome);
                removerContato(hash_table, nome);
                break;
            case 4:
                exibirContatos(hash_table);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
