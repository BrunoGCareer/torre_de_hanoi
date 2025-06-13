TORRE DE HANOI - ANÁLISE TÉCNICA COM LOCALIZAÇÃO DAS FUNÇÕES

-----------------------------------------------------------
1. inicializar_jogo() - Linhas 63-72
-----------------------------------------------------------
void inicializar_jogo(Torre *torres[], int num_discos) {
    for (int i = 0; i < 3; i++) {                         // Linha 63
        torres[i] = criar_torre(num_discos);               // Chama criar_torre() na linha 35
    }
    
    for (int i = num_discos; i > 0; i--) {                // Linha 67
        Disco disco = {i};                                 // Linha 68
        empilhar(torres[0], disco);                       // Chama empilhar() na linha 48
    }
}

Key Points:
- Inicialização na linha 63
- Loop de discos: linhas 67-71
- Dependências: criar_torre() (linha 35) e empilhar() (linha 48)

-----------------------------------------------------------
2. mover_disco() - Linhas 95-105
-----------------------------------------------------------
int mover_disco(Torre *origem, Torre *destino) {          // Linha 95
    if (torre_vazia(origem)) return 0;                    // Chama torre_vazia() na linha 41
    
    Disco topo_origem = ver_topo(origem);                 // Chama ver_topo() na linha 60
    Disco topo_destino = ver_topo(destino);               // linha 99
    
    if (!torre_vazia(destino) &&                          // linha 101
        topo_origem.tamanho > topo_destino.tamanho) {
        return 0;
    }
    
    empilhar(destino, desempilhar(origem));               // linha 104
    return 1;
}

Critical Path:
- Validação na linha 101
- Movimento na linha 104
- Chama 4 funções auxiliares (linhas 41, 48, 53, 60)

-----------------------------------------------------------
3. jogo_completo() - Linhas 107-109
-----------------------------------------------------------
int jogo_completo(Torre *torres[], int num_discos) {      // Linha 107
    return torres[2]->topo == num_discos - 1;             // Linha 108
}

Observações:
- Implementação concisa na linha 108
- Acesso direto à torre C (índice 2)

-----------------------------------------------------------
4. salvar_historico() - Linhas 178-188
-----------------------------------------------------------
void salvar_historico(Historico **lista, const char *nome, // Linha 178
                     int movimentos, int discos) {
    Historico *novo = (Historico*)malloc(sizeof(Historico));
    strncpy(novo->nome, nome, sizeof(novo->nome)-1);      // Linha 181
    novo->movimentos = movimentos;                        // Linha 182
    novo->discos = discos;                                // Linha 183
    obter_data_hora(novo->data);                          // Chama linha 132
    novo->proximo = NULL;
    
    inserir_ordenado(lista, novo);                       // Chama linha 143
    salvar_arquivo_historico(*lista);                    // Chama linha 168
}

Fluxo:
- Alocação: linha 179
- Atribuições: linhas 181-183
- Inserção: linha 186
- Persistência: linha 187

-----------------------------------------------------------
5. mostrar_torres() - Linhas 74-88
-----------------------------------------------------------
void mostrar_torres(Torre *torres[], int num_discos) {    // Linha 74 
    int largura_coluna = num_discos + 2;                  // Linha 75
    
    for (int i = num_discos - 1; i >= 0; i--) {          // Linha 77
        for (int j = 0; j < 3; j++) {                    // Linha 78
            if (i <= torres[j]->topo) {                  // Linha 79
                printf(" %*d ", largura_coluna, 
                      torres[j]->discos[i].tamanho);
            } else {
                printf(" %*s ", largura_coluna, "|");    // Linha 83
            }
        }
        printf("\n");
    }
    
    // Rótulos A/B/C
    for (int j = 0; j < 3; j++) {                       // Linha 87
        printf(" %*c ", largura_coluna, 'A' + j);        // Linha 88
    }
    printf("\n");
}

Renderização:
- Largura: linha 75
- Loop principal: linhas 77-86
- Rótulos: s 87-89
