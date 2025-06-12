TORRE DE HANOI - ANÁLISE TÉCNICA DAS FUNÇÕES PRINCIPAIS

-----------------------------------------------------------
1. inicializar_jogo(Torre *torres[], int num_discos)
-----------------------------------------------------------
Implementação:
void inicializar_jogo(Torre *torres[], int num_discos) {
    for (int i = 0; i < 3; i++) {
        torres[i] = criar_torre(num_discos);
    }
    
    for (int i = num_discos; i > 0; i--) {
        Disco disco = {i};
        empilhar(torres[0], disco);
    }
}

Detalhes:
- Aloca memória para 3 torres usando criar_torre()
- Preenche a torre A (índice 0) com discos em ordem decrescente
- Utiliza empilhar() para inserir cada disco
- Complexidade: O(n) para n discos

-----------------------------------------------------------
2. mover_disco(Torre *origem, Torre *destino)
-----------------------------------------------------------
Implementação:
int mover_disco(Torre *origem, Torre *destino) {
    if (torre_vazia(origem)) return 0;
    
    Disco topo_origem = ver_topo(origem);
    Disco topo_destino = ver_topo(destino);
    
    if (!torre_vazia(destino) && 
        topo_origem.tamanho > topo_destino.tamanho) {
        return 0;
    }
    
    empilhar(destino, desempilhar(origem));
    return 1;
}

Detalhes:
- Verifica regras do jogo:
  1. Origem não pode estar vazia (torre_vazia())
  2. Disco de origem deve ser menor que o do topo do destino
- Retorna 1 (sucesso) ou 0 (falha)
- Operações realizadas:
  - ver_topo() para inspeção
  - desempilhar() + empilhar() para movimento

-----------------------------------------------------------
3. jogo_completo(Torre *torres[], int num_discos)
-----------------------------------------------------------
Implementação:
int jogo_completo(Torre *torres[], int num_discos) {
    return torres[2]->topo == num_discos - 1;
}

Detalhes:
- Verifica se a torre C (índice 2) contém todos os discos
- Compara topo da torre com num_discos - 1 (indexação zero)
- Complexidade: O(1)

-----------------------------------------------------------
4. salvar_historico()
-----------------------------------------------------------
Fluxo:
1. Cria novo nó Historico
2. Preenche com:
   - nome (string)
   - movimentos (int)
   - discos (int)
   - data/hora (obter_data_hora())
3. Insere ordenado na lista encadeada
4. Escreve no arquivo historico.txt

Detalhes:
- Formato do arquivo:
  "nome - movimentos - discos - data\n"
- Ordenação:
  - Primário: discos (crescente)
  - Secundário: movimentos (crescente)

-----------------------------------------------------------
5. mostrar_torres()
-----------------------------------------------------------
Implementação:
void mostrar_torres(Torre *torres[], int num_discos) {
    int largura_coluna = num_discos + 2;
    
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

Detalhes:
- Exibe torres verticalmente com alinhamento correto
- Formatação dinâmica baseada em num_discos
- Mostra:
  - Discos (números) ou pinos ("|") quando vazio
  - Identificação das torres (A, B, C)
