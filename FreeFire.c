#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// -----------------------------------------------------------------
// 💡 CONSTANTES E ENUMS
// -----------------------------------------------------------------

#define CAPACIDADE_MAXIMA 10
#define MAX_NOME 50
#define MAX_TIPO 30

// 💡 Conceito: Enumeração para critérios de ordenação
typedef enum {
    NAO_ORDENADO,
    ORDENADO_NOME,
    ORDENADO_TIPO,
    ORDENADO_PRIORIDADE
} CriterioOrdenacao;

// -----------------------------------------------------------------
// 💡 ESTRUTURAS
// -----------------------------------------------------------------

// Estrutura atualizada com o campo 'prioridade'
struct Item {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
    int prioridade; // 1 (Mais baixa) a 5 (Mais alta)
    int ocupado;
};

// Estrutura para manter o estado do inventário (Passagem por referência)
struct Inventario {
    struct Item mochila[CAPACIDADE_MAXIMA];
    int total_itens;
    CriterioOrdenacao estado_ordenacao; // Estado atual de ordenação
};

// -----------------------------------------------------------------
// PROTÓTIPOS
// -----------------------------------------------------------------
void limpar_buffer_teclado();
void formatar_entrada(char *str);
void exibir_menu();

// Funções de Inventário
void adicionar_item(struct Inventario *inv);
void listar_itens(const struct Inventario *inv);
void remover_item(struct Inventario *inv);
void processar_busca_sequencial(const struct Inventario *inv);
void processar_busca_binaria(const struct Inventario *inv);

// Funções de Ordenação
void exibir_menu_ordenacao();
void ordenar_inventario(struct Inventario *inv, int criterio_escolhido);
long int insertion_sort(struct Item arr[], int n, int criterio);

// -----------------------------------------------------------------
// FUNÇÕES AUXILIARES E DE INVENTÁRIO
// -----------------------------------------------------------------

void limpar_buffer_teclado() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void formatar_entrada(char *str) {
    str[strcspn(str, "\n")] = 0;
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

void exibir_menu() {
    printf("\n--- 🎒 INVENTÁRIO MESTRE ---\n");
    printf("1. Adicionar Item\n");
    printf("2. Remover Item\n");
    printf("3. Listar Itens\n");
    printf("4. Busca Sequencial (por Nome)\n");
    printf("5. Ordenar Mochila\n");
    printf("6. Busca Binária (por Nome)\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
}

// -----------------------------------------------------------------
// 1. Adicionar Item
// -----------------------------------------------------------------
void adicionar_item(struct Inventario *inv) {
    if (inv->total_itens >= CAPACIDADE_MAXIMA) {
        printf("\n🚫 A mochila está cheia.\n");
        return;
    }

    int i;
    for (i = 0; i < CAPACIDADE_MAXIMA; i++) {
        if (inv->mochila[i].ocupado == 0) {
            break;
        }
    }
    
    printf("\n--- Adicionar Item ---\n");
    
    printf("Nome do Item: ");
    fgets(inv->mochila[i].nome, MAX_NOME, stdin);
    formatar_entrada(inv->mochila[i].nome); 

    printf("Tipo do Item: ");
    fgets(inv->mochila[i].tipo, MAX_TIPO, stdin);
    formatar_entrada(inv->mochila[i].tipo);
    
    printf("Quantidade: ");
    if (scanf("%d", &inv->mochila[i].quantidade) != 1 || inv->mochila[i].quantidade <= 0) {
        printf("⚠️ Quantidade inválida. Item não adicionado.\n");
        limpar_buffer_teclado();
        return;
    }
    
    // Leitura da Prioridade
    printf("Prioridade (1 a 5, 5 é a mais alta): ");
    if (scanf("%d", &inv->mochila[i].prioridade) != 1 || inv->mochila[i].prioridade < 1 || inv->mochila[i].prioridade > 5) {
        printf("⚠️ Prioridade inválida. Item não adicionado.\n");
        limpar_buffer_teclado();
        return;
    }

    limpar_buffer_teclado(); 

    inv->mochila[i].ocupado = 1;
    inv->total_itens++;
    
    // 💡 Reinicia o estado de ordenação
    inv->estado_ordenacao = NAO_ORDENADO;
    
    printf("\n✅ Item '%s' adicionado com sucesso.\n", inv->mochila[i].nome);
}

// -----------------------------------------------------------------
// 3. Listar Itens
// -----------------------------------------------------------------
void listar_itens(const struct Inventario *inv) {
    printf("\n--- 📜 INVENTÁRIO ATUAL (Total: %d/%d) ---\n", inv->total_itens, CAPACIDADE_MAXIMA);
    
    // Exibe o estado de ordenação
    const char *status_ordem;
    switch (inv->estado_ordenacao) {
        case ORDENADO_NOME: status_ordem = "ORDENADA (Nome)"; break;
        case ORDENADO_TIPO: status_ordem = "ORDENADA (Tipo)"; break;
        case ORDENADO_PRIORIDADE: status_ordem = "ORDENADA (Prioridade)"; break;
        default: status_ordem = "NÃO ORDENADA"; break;
    }
    printf("   Status: %s\n", status_ordem);

    if (inv->total_itens == 0) {
        printf("   (Vazio)\n");
        return;
    }

    // Cabeçalho da tabela
    printf("%-5s | %-20s | %-15s | %-5s | %s\n", "ID", "Nome", "Tipo", "Qtd", "Prioridade");
    printf("------------------------------------------------------------\n");

    int id_display = 1;
    for (int i = 0; i < CAPACIDADE_MAXIMA; i++) {
        if (inv->mochila[i].ocupado == 1) {
            printf("%-5d | %-20s | %-15s | %-5d | %d\n", 
                   id_display,
                   inv->mochila[i].nome,
                   inv->mochila[i].tipo,
                   inv->mochila[i].quantidade,
                   inv->mochila[i].prioridade);
            id_display++;
        }
    }
    printf("------------------------------------------------------------\n");
}


// -----------------------------------------------------------------
// FUNÇÕES DE ORDENAÇÃO
// -----------------------------------------------------------------

// 💡 Conceito: Insertion Sort com Contador de Comparações
long int insertion_sort(struct Item arr[], int n, int criterio) {
    long int comparacoes = 0;
    int i, j;
    struct Item chave;

    for (i = 1; i < n; i++) {
        chave = arr[i];
        j = i - 1;

        while (j >= 0) {
            bool deve_trocar = false;
            
            // 1. Define o critério de comparação
            switch (criterio) {
                case ORDENADO_NOME:
                    // ORDENADO_NOME: A-Z
                    deve_trocar = (strcmp(arr[j].nome, chave.nome) > 0);
                    break;
                case ORDENADO_TIPO:
                    // ORDENADO_TIPO: A-Z, desempate por Nome
                    if (strcmp(arr[j].tipo, chave.tipo) > 0) {
                        deve_trocar = true;
                    } else if (strcmp(arr[j].tipo, chave.tipo) == 0) {
                        deve_trocar = (strcmp(arr[j].nome, chave.nome) > 0);
                    }
                    break;
                case ORDENADO_PRIORIDADE:
                    // ORDENADO_PRIORIDADE: 5-1 (descrescente), desempate por Nome (A-Z)
                    if (arr[j].prioridade < chave.prioridade) {
                        deve_trocar = true;
                    } else if (arr[j].prioridade == chave.prioridade) {
                         deve_trocar = (strcmp(arr[j].nome, chave.nome) > 0);
                    }
                    break;
            }
            
            comparacoes++; 

            if (deve_trocar) {
                arr[j + 1] = arr[j];
                j = j - 1;
            } else {
                break;
            }
        }
        arr[j + 1] = chave;
    }
    return comparacoes;
}

void exibir_menu_ordenacao() {
    printf("\n--- Escolher Critério de Ordenação ---\n");
    printf("1. Nome (A-Z)\n");
    printf("2. Tipo (A-Z, desempate por Nome)\n");
    printf("3. Prioridade (5-1, desempate por Nome)\n");
    printf("Sua escolha: ");
}

// 5. Função de Ordenação
void ordenar_inventario(struct Inventario *inv, int criterio_escolhido) {
    if (inv->total_itens <= 1) {
        printf("\n✅ Não é necessário ordenar (0 ou 1 item).\n");
        inv->estado_ordenacao = (CriterioOrdenacao)criterio_escolhido;
        return;
    }
    
    // Cria uma cópia compactada dos itens ocupados para ordenar
    struct Item temp_arr[inv->total_itens];
    int k = 0;
    for (int i = 0; i < CAPACIDADE_MAXIMA; i++) {
        if (inv->mochila[i].ocupado == 1) {
            temp_arr[k++] = inv->mochila[i];
        }
    }

    // Chama o Insertion Sort
    long int comparacoes = insertion_sort(temp_arr, inv->total_itens, criterio_escolhido);

    // Sobrescreve o inventário, mantendo apenas os itens ordenados e compactados
    for (int i = 0; i < CAPACIDADE_MAXIMA; i++) {
        inv->mochila[i].ocupado = 0; 
    }
    for (int i = 0; i < inv->total_itens; i++) {
        inv->mochila[i] = temp_arr[i]; 
        inv->mochila[i].ocupado = 1;
    }

    // 💡 Atualiza o estado da mochila
    inv->estado_ordenacao = (CriterioOrdenacao)criterio_escolhido;
    
    printf("\n✅ Mochila ordenada com sucesso.\n");
    printf("   Comparações realizadas: %ld\n", comparacoes);
}


// -----------------------------------------------------------------
// FUNÇÕES DE BUSCA
// -----------------------------------------------------------------

// Função de Busca Sequencial (Usada por 'Remover' e 'Busca Sequencial')
int buscar_sequencial(const struct Inventario *inv, const char *nome_busca) {
    char nome_formatado[MAX_NOME];
    strcpy(nome_formatado, nome_busca);
    formatar_entrada(nome_formatado);

    for (int i = 0; i < CAPACIDADE_MAXIMA; i++) {
        if (inv->mochila[i].ocupado == 1 && strcmp(inv->mochila[i].nome, nome_formatado) == 0) {
            return i; 
        }
    }
    return -1;
}

void processar_busca_sequencial(const struct Inventario *inv) {
    char nome_busca[MAX_NOME];
    printf("\n--- 🔍 Busca Sequencial ---\n");
    printf("Digite o NOME do item: ");
    fgets(nome_busca, MAX_NOME, stdin);
    nome_busca[strcspn(nome_busca, "\n")] = 0; 

    int indice = buscar_sequencial(inv, nome_busca);

    if (indice != -1) {
        printf("\n✅ Item encontrado (Sequencial):\n");
        printf("  - Nome:      %s\n", inv->mochila[indice].nome);
        printf("  - Tipo:      %s\n", inv->mochila[indice].tipo);
        printf("  - Quantidade: %d\n", inv->mochila[indice].quantidade);
        printf("  - Prioridade: %d\n", inv->mochila[indice].prioridade);
    } else {
        printf("\n❌ Item '%s' não encontrado.\n", nome_busca);
    }
}

// 💡 Conceito: Busca Binária
int buscar_binaria(const struct Inventario *inv, const char *nome_busca) {
    char nome_formatado[MAX_NOME];
    strcpy(nome_formatado, nome_busca);
    formatar_entrada(nome_formatado);

    int inicio = 0;
    int fim = inv->total_itens - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        
        // Compara com o item no meio do array compactado
        int comparacao = strcmp(inv->mochila[meio].nome, nome_formatado);

        if (comparacao == 0) {
            return meio; // Item encontrado
        } else if (comparacao < 0) {
            inicio = meio + 1; // Busca na metade superior
        } else {
            fim = meio - 1; // Busca na metade inferior
        }
    }

    return -1; 
}

// 6. Função para processar a busca binária
void processar_busca_binaria(const struct Inventario *inv) {
    printf("\n--- 🚀 Busca Binária ---\n");

    // 💡 Conceito: Validação de pré-requisito
    if (inv->estado_ordenacao != ORDENADO_NOME) {
        printf("\n⚠️ ERRO: A busca binária exige que a mochila esteja ordenada por NOME (A-Z).\n");
        printf("   Por favor, ordene por Nome (Opção 5 -> 1) antes de prosseguir.\n");
        return;
    }
    
    char nome_busca[MAX_NOME];
    printf("Digite o NOME do item: ");
    fgets(nome_busca, MAX_NOME, stdin);
    nome_busca[strcspn(nome_busca, "\n")] = 0;

    int indice = buscar_binaria(inv, nome_busca);

    if (indice != -1) {
        printf("\n✅ Item encontrado (Binária):\n");
        printf("  - Nome:      %s\n", inv->mochila[indice].nome);
        printf("  - Tipo:      %s\n", inv->mochila[indice].tipo);
        printf("  - Quantidade: %d\n", inv->mochila[indice].quantidade);
        printf("  - Prioridade: %d\n", inv->mochila[indice].prioridade);
    } else {
        printf("\n❌ Item '%s' não encontrado.\n", nome_busca);
    }
}

// 2. Remover item (Utiliza a busca sequencial)
void remover_item(struct Inventario *inv) {
    char nome_remover[MAX_NOME];
    
    if (inv->total_itens == 0) {
        printf("\n🚫 Inventário vazio. Não há o que remover.\n");
        return;
    }

    printf("\n--- Remover Item ---\n");
    printf("Digite o NOME do item a ser removido: ");
    fgets(nome_remover, MAX_NOME, stdin);
    
    int indice = buscar_sequencial(inv, nome_remover);

    if (indice != -1) {
        // Removendo: Apenas marca a posição como livre
        inv->mochila[indice].ocupado = 0; 
        inv->total_itens--;
        
        // O vetor agora está descompactado, o estado de ordenação é perdido
        inv->estado_ordenacao = NAO_ORDENADO;
        
        // 💡 Boas Práticas: Compactar o array após a remoção.
        // Como o vetor não é mais compactado após a remoção, é melhor forçar uma reordenação/compactação.
        // Para simplificar no Nível Mestre, vamos apenas desordenar o estado, forçando o usuário a reordenar para usar a busca binária.

        printf("\n🗑️ Item '%s' removido com sucesso.\n", inv->mochila[indice].nome);
    } else {
        printf("\n❌ Item '%s' não encontrado.\n", nome_remover);
    }
}


// -----------------------------------------------------------------
// FUNÇÃO PRINCIPAL (main)
// -----------------------------------------------------------------
int main() {
    struct Inventario inv; 
    inv.total_itens = 0;
    inv.estado_ordenacao = NAO_ORDENADO;

    // Inicializa todos os slots como vazios
    for (int i = 0; i < CAPACIDADE_MAXIMA; i++) {
        inv.mochila[i].ocupado = 0;
    }
    
    int opcao;
    
    do {
        listar_itens(&inv);
        exibir_menu();
        
        if (scanf("%d", &opcao) != 1) {
            opcao = -1;
        }
        limpar_buffer_teclado();

        switch (opcao) {
            case 1:
                adicionar_item(&inv);
                break;
            case 2:
                remover_item(&inv);
                break;
            case 3:
                printf("\nReexibindo a lista...\n"); 
                break;
            case 4:
                processar_busca_sequencial(&inv);
                break;
            case 5: { 
                exibir_menu_ordenacao();
                int criterio;
                if (scanf("%d", &criterio) == 1 && criterio >= 1 && criterio <= 3) {
                    ordenar_inventario(&inv, criterio);
                } else {
                    printf("\n⚠️ Critério inválido.\n");
                }
                limpar_buffer_teclado();
                break;
            }
            case 6:
                processar_busca_binaria(&inv);
                break;
            case 0:
                printf("\nSaindo do inventário Mestre. Até logo!\n");
                break;
            default:
                printf("\n⚠️ Opção inválida. Tente novamente (0-6).\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}