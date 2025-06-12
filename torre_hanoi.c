#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// Estrutura para representar um disco
typedef struct Disco {
    int tamanho;
} Disco;

// Estrutura para representar uma torre (pilha)
typedef struct Torre {
    Disco *discos;
    int topo;
    int capacidade;
} Torre;

// Estrutura para armazenar histórico de partidas
typedef struct Historico {
    char nome[50];
    int movimentos;
    int discos;
    char data[20];
    struct Historico *proximo;
} Historico;

// Funções para manipulação das torres
Torre* criar_torre(int capacidade) {
    Torre *torre = (Torre*)malloc(sizeof(Torre));
    torre->capacidade = capacidade;
    torre->topo = -1;
    torre->discos = (Disco*)malloc(capacidade * sizeof(Disco));
    return torre;
}

int torre_vazia(Torre *torre) {
    return torre->topo == -1;
}

int torre_cheia(Torre *torre) {
    return torre->topo == torre->capacidade - 1;
}

void empilhar(Torre *torre, Disco disco) {
    if (torre_cheia(torre)) return;
    torre->discos[++torre->topo] = disco;
}

Disco desempilhar(Torre *torre) {
    Disco disco_vazio = {0};
    if (torre_vazia(torre)) return disco_vazio;
    return torre->discos[torre->topo--];
}

Disco ver_topo(Torre *torre) {
    Disco disco_vazio = {0};
    if (torre_vazia(torre)) return disco_vazio;
    return torre->discos[torre->topo];
}

// Funções para o jogo
void inicializar_jogo(Torre *torres[], int num_discos) {
    for (int i = 0; i < 3; i++) {
        torres[i] = criar_torre(num_discos);
    }
    
    for (int i = num_discos; i > 0; i--) {
        Disco disco = {i};
        empilhar(torres[0], disco);
    }
}

void mostrar_torres(Torre *torres[], int num_discos) {
    int largura_coluna = num_discos + 2;

    printf("\n");
    for (int i = num_discos - 1; i >= 0; i--) {
        for (int j = 0; j < 3; j++) {
            if (i <= torres[j]->topo) {
                printf(" %*d ", largura_coluna, torres[j]->discos[i].tamanho);
            } else {
                printf(" %*s ", largura_coluna, "|");
            }
        }
        printf("\n");
    }
    for (int j = 0; j < 3; j++) {
        printf(" %*c ", largura_coluna, 'A' + j);
    }
    printf("\n");
}

int mover_disco(Torre *origem, Torre *destino) {
    if (torre_vazia(origem)) return 0;
    Disco topo_origem = ver_topo(origem);
    Disco topo_destino = ver_topo(destino);
    if (!torre_vazia(destino) && topo_origem.tamanho > topo_destino.tamanho) return 0;
    empilhar(destino, desempilhar(origem));
    return 1;
}

int jogo_completo(Torre *torres[], int num_discos) {
    return torres[2]->topo == num_discos - 1;
}

void liberar_torres(Torre *torres[]) {
    for (int i = 0; i < 3; i++) {
        free(torres[i]->discos);
        free(torres[i]);
    }
}

// Funções para o histórico
void obter_data_hora(char *buffer) {
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 20, "%d/%m/%Y %H:%M", timeinfo);
}

// Insere ordenado por discos e movimentos (crescente)
void inserir_ordenado(Historico **lista, Historico *novo) {
    if (*lista == NULL ||
        novo->discos < (*lista)->discos ||
        (novo->discos == (*lista)->discos && novo->movimentos < (*lista)->movimentos)) {
        novo->proximo = *lista;
        *lista = novo;
        return;
    }
    Historico *atual = *lista;
    while (atual->proximo != NULL) {
        if (novo->discos < atual->proximo->discos) break;
        if (novo->discos == atual->proximo->discos && novo->movimentos < atual->proximo->movimentos) break;
        atual = atual->proximo;
    }
    novo->proximo = atual->proximo;
    atual->proximo = novo;
}

// Carrega histórico do arquivo reorganizando e juntando grupos automaticamente
void carregar_historico(Historico **lista) {
    FILE *arquivo = fopen("historico.txt", "r");
    if (!arquivo) return;
    char linha[200];
    while (fgets(linha, sizeof(linha), arquivo)) {
        Historico *novo = (Historico*)malloc(sizeof(Historico));
        if (!novo) continue;
        // Parse linha formato: nome - movimentos - discos - data
        char *token = strtok(linha, "-");
        if (!token) { free(novo); continue; }
        strncpy(novo->nome, token, sizeof(novo->nome)-1);
        novo->nome[sizeof(novo->nome)-1] = '\0';
        token = strtok(NULL, "-");
        if (!token) { free(novo); continue; }
        novo->movimentos = atoi(token);
        token = strtok(NULL, "-");
        if (!token) { free(novo); continue; }
        novo->discos = atoi(token);
        token = strtok(NULL, "\n");
        if (!token) { free(novo); continue; }
        strncpy(novo->data, token, sizeof(novo->data)-1);
        novo->data[sizeof(novo->data)-1] = '\0';
        novo->proximo = NULL;
        inserir_ordenado(lista, novo); // INSERE ORDENADO EM LISTA AGRUPADA
    }
    fclose(arquivo);
}

void salvar_arquivo_historico(Historico *lista) {
    FILE *arquivo = fopen("historico.txt", "w");
    if (!arquivo) return;
    while (lista != NULL) {
        fprintf(arquivo, "%s - %d - %d - %s\n", lista->nome, lista->movimentos, lista->discos, lista->data);
        lista = lista->proximo;
    }
    fclose(arquivo);
}

void salvar_historico(Historico **lista, const char *nome, int movimentos, int discos) {
    Historico *novo = (Historico*)malloc(sizeof(Historico));
    if (!novo) return;
    strncpy(novo->nome, nome, sizeof(novo->nome)-1);
    novo->nome[sizeof(novo->nome)-1] = '\0';
    novo->movimentos = movimentos;
    novo->discos = discos;
    obter_data_hora(novo->data);
    novo->proximo = NULL;
    inserir_ordenado(lista, novo);
    salvar_arquivo_historico(*lista);
}

void mostrar_historico(Historico *lista) {
    if (lista == NULL) {
        printf("\nNenhum histórico encontrado.\n");
        return;
    }
    int discos_atual = -1;
    while (lista != NULL) {
        if (lista->discos != discos_atual) {
            discos_atual = lista->discos;
            printf("\n=== Histórico para %d disco(s) ===\n", discos_atual);
            printf("%-20s %-10s %-20s\n", "Jogador", "Movimentos", "Data/Hora");
            printf("------------------------------------------------------------\n");
        }
        printf("%-20s %-10d %-20s\n", lista->nome, lista->movimentos, lista->data);
        lista = lista->proximo;
    }
}

void liberar_historico(Historico *lista) {
    while (lista != NULL) {
        Historico *temp = lista->proximo;
        free(lista);
        lista = temp;
    }
}

int main() {
    Torre *torres[3] = {NULL, NULL, NULL};
    Historico *historico = NULL;
    carregar_historico(&historico);
    int num_discos = 3;
    char nome_jogador[50] = "Anonimo";
    int movimentos = 0;
    int opcao;
    printf("=== TORRE DE HANOI ===\nUm jogo matematico classico!\n\n");
    do {
        printf("\nMenu Principal:\n");
        printf("1. Jogar\n");
        printf("2. Definir numero de discos (atual: %d)\n", num_discos);
        printf("3. Definir nome do jogador (atual: %s)\n", nome_jogador);
        printf("4. Ver historico completo\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();
        switch (opcao) {
        case 1: {
            inicializar_jogo(torres, num_discos);
            movimentos = 0;
            int jogando = 1;
            char entrada[10];
            while (jogando) {
                mostrar_torres(torres, num_discos);
                printf("\nMovimentos: %d\n", movimentos);
                printf("Digite origem e destino (ex: AC), 'R' para reiniciar, 'M' para menu: ");
                if (!fgets(entrada, sizeof(entrada), stdin)) {
                    printf("Erro na entrada. Saindo do jogo.\n");
                    liberar_torres(torres);
                    jogando = 0;
                    break;
                }
                char origem = toupper(entrada[0]);
                if (origem == 'R') {
                    liberar_torres(torres);
                    inicializar_jogo(torres, num_discos);
                    movimentos = 0;
                    printf("\nJogo reiniciado!\n");
                    continue;
                }
                if (origem == 'M') {
                    liberar_torres(torres);
                    jogando = 0;
                    break;
                }
                char destino = toupper(entrada[1]);
                if (origem < 'A' || origem > 'C' || destino < 'A' || destino > 'C') {
                    printf("Torres invalidas! Use A, B ou C.\n");
                    continue;
                }
                if (mover_disco(torres[origem - 'A'], torres[destino - 'A'])) {
                    movimentos++;
                    printf("Movimento: %c -> %c\n", origem, destino);
                } else {
                    printf("Movimento invalido!\n");
                }
                if (jogo_completo(torres, num_discos)) {
                    mostrar_torres(torres, num_discos);
                    printf("\nParabens! Voce completou o jogo em %d movimentos.\n", movimentos);
                    salvar_historico(&historico, nome_jogador, movimentos, num_discos);
                    liberar_torres(torres);
                    jogando = 0;
                }
            }
            break;
        }
        case 2:
            printf("\nDigite o numero de discos (1-8): ");
            scanf("%d", &num_discos);
            getchar();
            if (num_discos < 1 || num_discos > 8) {
                printf("Numero invalido. Usando padrao (3).\n");
                num_discos = 3;
            }
            break;
        case 3:
            printf("\nDigite seu nome (max 50 caracteres): ");
            fgets(nome_jogador, sizeof(nome_jogador), stdin);
            nome_jogador[strcspn(nome_jogador, "\n")] = '\0';
            break;
        case 4:
            mostrar_historico(historico);
            break;
        case 0:
            printf("\nObrigado por jogar!\n");
            break;
        default:
            printf("\nOpcao invalida!\n");
            break;
        }
    } while (opcao != 0);
    liberar_historico(historico);
    return 0;
}
