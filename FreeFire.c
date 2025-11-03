#include <stdio.h>
#include <string.h>
#include <ctype.h> // Para tolower
#include <stdbool.h> // Para usar o tipo bool

// -----------------------------------------------------------------
// 💡 CONSTANTES E ESTRUTURAS
// -----------------------------------------------------------------

// Capacidade máxima da mochila
#define CAPACIDADE_MAXIMA 10 
#define MAX_NOME 50
#define MAX_TIPO 30

// Estrutura para representar um item
struct Item {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
    // Flag para marcar se a posição está ocupada ou "vazia"
    int ocupado; // 1 se o item existe, 0 se a posição está livre
};

// -----------------------------------------------------------------
// FUNÇÕES AUXILIARES
// -----------------------------------------------------------------

// Limpa o buffer de entrada do teclado
void limpar_buffer_teclado() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Converte a string para minúsculas e remove o '\n' (se houver)
void formatar_entrada(char *str) {
    // 1. Remove o newline deixado pelo fgets (se existir)
    str[strcspn(str, "\n")] = 0;
    
    // 2. Converte para minúsculas para a busca ser case-insensitive
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// -----------------------------------------------------------------
// FUNÇÕES PRINCIPAIS DO INVENTÁRIO
// -----------------------------------------------------------------

// Função para exibir o menu
void exibir_menu() {
    printf("\n--- 🎒 INVENTÁRIO AVENTUREIRO ---\n");
    printf("1. Adicionar Item\n");
    printf("2. Remover Item\n");
    printf("3. Listar Itens\n");
    printf("4. Buscar Item por Nome\n"); // 🆕 Nova opção
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
}

// Função para buscar um item por nome (Busca Sequencial)
// 💡 Conceito: Retorna o índice onde o item foi encontrado, ou -1 se não encontrar.
int buscar_item_por_nome(const struct Item inventario[], const char *nome_busca) {
    
    // Cria uma cópia formatada do nome de busca para garantir a comparação
    char nome_formatado[MAX_NOME];
    strcpy(nome_formatado, nome_busca);
    formatar_entrada(nome_formatado);

    // 💡 Conceito: Busca Sequencial
    // Percorre o vetor do início ao fim.
    for (int i = 0; i < CAPACIDADE_MAXIMA; i++) {
        // Verifica se a posição está ocupada E se o nome coincide
        if (inventario[i].ocupado == 1 && strcmp(inventario[i].nome, nome_formatado) == 0) {
            return i; // Item encontrado no índice 'i'
        }
    }
    
    return -1; // Item não encontrado
}


// 4. Implementação da busca
void processar_busca(const struct Item inventario[]) {
    char nome_busca[MAX_NOME];

    printf("\n--- 🔍 Buscar Item ---\n");
    printf("Digite o NOME do item a ser buscado: ");
    fgets(nome_busca, MAX_NOME, stdin);
    
    // formatar_entrada é chamado dentro de buscar_item_por_nome, mas
    // precisamos remover o '\n' da cópia aqui para a mensagem de erro.
    nome_busca[strcspn(nome_busca, "\n")] = 0;

    // Encontra o índice (ou -1)
    int indice = buscar_item_por_nome(inventario, nome_busca);

    if (indice != -1) {
        // 💡 Saída: Exibição detalhada do item encontrado
        printf("\n✅ Item ENCONTRADO:\n");
        printf("  - Nome:      %s\n", inventario[indice].nome);
        printf("  - Tipo:      %s\n", inventario[indice].tipo);
        printf("  - Quantidade: %d\n", inventario[indice].quantidade);
    } else {
        printf("\n❌ Item '%s' não encontrado no inventário.\n", nome_busca);
    }
}

// 1. Adicionar item
void adicionar_item(struct Item inventario[], int *total_itens) {
    if (*total_itens >= CAPACIDADE_MAXIMA) {
        printf("\n🚫 A mochila está cheia! Remova um item antes de adicionar.\n");
        return;
    }

    int i;
    for (i = 0; i < CAPACIDADE_MAXIMA; i++) {
        if (inventario[i].ocupado == 0) {
            break; 
        }
    }
    
    printf("\n--- Adicionar Item ---\n");
    
    printf("Nome do Item: ");
    fgets(inventario[i].nome, MAX_NOME, stdin);
    formatar_entrada(inventario[i].nome); 

    printf("Tipo do Item: ");
    fgets(inventario[i].tipo, MAX_TIPO, stdin);
    formatar_entrada(inventario[i].tipo);
    
    printf("Quantidade: ");
    if (scanf("%d", &inventario[i].quantidade) != 1 || inventario[i].quantidade <= 0) {
        printf("⚠️ Quantidade inválida. Item não adicionado.\n");
        limpar_buffer_teclado();
        inventario[i].ocupado = 0; 
        return;
    }
    limpar_buffer_teclado(); 

    inventario[i].ocupado = 1;
    (*total_itens)++;
    
    printf("\n✅ Item '%s' adicionado com sucesso.\n", inventario[i].nome);
}

// 2. Remover item
void remover_item(struct Item inventario[], int *total_itens) {
    char nome_remover[MAX_NOME];
    
    if (*total_itens == 0) {
        printf("\n🚫 Inventário vazio. Não há o que remover.\n");
        return;
    }

    printf("\n--- Remover Item ---\n");
    printf("Digite o NOME do item a ser removido: ");
    fgets(nome_remover, MAX_NOME, stdin);
    
    // Usa a função de busca para encontrar o índice
    int indice = buscar_item_por_nome(inventario, nome_remover);

    if (indice != -1) {
        // Removendo: Apenas marca a posição como livre
        inventario[indice].ocupado = 0; 
        (*total_itens)--;
        printf("\n🗑️ Item '%s' removido com sucesso.\n", inventario[indice].nome);
    } else {
        printf("\n❌ Item '%s' não encontrado no inventário.\n", nome_remover);
    }
}

// 3. Listar itens
void listar_itens(const struct Item inventario[], int total_itens) {
    printf("\n--- 📜 INVENTÁRIO ATUAL (Total: %d/%d) ---\n", total_itens, CAPACIDADE_MAXIMA);
    
    if (total_itens == 0) {
        printf("   (Vazio)\n");
        return;
    }

    // Cabeçalho da tabela
    printf("%-5s | %-20s | %-15s | %s\n", "ID", "Nome", "Tipo", "Qtd");
    printf("----------------------------------------------------\n");

    int id_display = 1; // ID para exibição (1, 2, 3...)
    
    for (int i = 0; i < CAPACIDADE_MAXIMA; i++) {
        if (inventario[i].ocupado == 1) {
            printf("%-5d | %-20s | %-15s | %d\n", 
                   id_display,
                   inventario[i].nome,
                   inventario[i].tipo,
                   inventario[i].quantidade);
            id_display++;
        }
    }
    printf("----------------------------------------------------\n");
}

// -----------------------------------------------------------------
// FUNÇÃO PRINCIPAL (main)
// -----------------------------------------------------------------
int main() {
    struct Item mochila[CAPACIDADE_MAXIMA]; 
    int total_itens = 0;

    // Inicializa a mochila (todos os slots vazios)
    for (int i = 0; i < CAPACIDADE_MAXIMA; i++) {
        mochila[i].ocupado = 0;
    }
    
    int opcao;
    
    do {
        listar_itens(mochila, total_itens);
        exibir_menu();
        
        if (scanf("%d", &opcao) != 1) {
            opcao = -1; // Opção inválida
        }
        limpar_buffer_teclado();

        switch (opcao) {
            case 1:
                adicionar_item(mochila, &total_itens);
                break;
            case 2:
                remover_item(mochila, &total_itens);
                break;
            case 3:
                printf("\nReexibindo a lista...\n"); 
                break;
            case 4:
                processar_busca(mochila); // 🆕 Chama a nova função de busca
                break;
            case 0:
                printf("\nSaindo do inventário. Até logo!\n");
                break;
            default:
                printf("\n⚠️ Opção inválida. Tente novamente (0-4).\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}