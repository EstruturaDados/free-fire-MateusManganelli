#include <stdio.h>
#include <string.h>
#include <ctype.h> // Para tolower

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
    // Usamos um flag para marcar se a posição está ocupada ou "vazia"
    int ocupado; // 1 se o item existe, 0 se a posição está livre
};

// -----------------------------------------------------------------
// FUNÇÕES AUXILIARES
// -----------------------------------------------------------------

// Limpa o buffer de entrada do teclado após scanf para evitar problemas com fgets
void limpar_buffer_teclado() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Converte a string para minúsculas e remove o '\n' (se houver)
void formatar_entrada(char *str) {
    // 1. Remove o newline deixado pelo fgets (se existir)
    str[strcspn(str, "\n")] = 0;
    
    // 2. Converte para minúsculas para facilitar a busca (case-insensitive)
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// -----------------------------------------------------------------
// FUNÇÕES PRINCIPAIS DO INVENTÁRIO
// -----------------------------------------------------------------

// Função para exibir o menu
void exibir_menu() {
    printf("\n--- 🎒 INVENTÁRIO BÁSICO ---\n");
    printf("1. Adicionar Item\n");
    printf("2. Remover Item\n");
    printf("3. Listar Itens\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
}

// 1. Adicionar item
void adicionar_item(struct Item inventario[], int *total_itens) {
    if (*total_itens >= CAPACIDADE_MAXIMA) {
        printf("\n🚫 A mochila está cheia! Remova um item antes de adicionar.\n");
        return;
    }

    // Encontra a primeira posição livre (ocupado == 0)
    int i;
    for (i = 0; i < CAPACIDADE_MAXIMA; i++) {
        if (inventario[i].ocupado == 0) {
            break; // Encontrou um slot
        }
    }
    
    printf("\n--- Adicionar Item ---\n");
    
    // Leitura do Nome (usamos fgets para permitir espaços)
    printf("Nome do Item: ");
    fgets(inventario[i].nome, MAX_NOME, stdin);
    formatar_entrada(inventario[i].nome); // Formata (minúsculas e sem '\n')

    // Leitura do Tipo
    printf("Tipo do Item: ");
    fgets(inventario[i].tipo, MAX_TIPO, stdin);
    formatar_entrada(inventario[i].tipo);
    
    // Leitura da Quantidade (usamos scanf para inteiros)
    printf("Quantidade: ");
    // Sempre verificamos o retorno de scanf, embora aqui, no nível novato, seja simplificado
    if (scanf("%d", &inventario[i].quantidade) != 1 || inventario[i].quantidade <= 0) {
        printf("⚠️ Quantidade inválida. Item não adicionado.\n");
        limpar_buffer_teclado();
        // Marca o slot como vazio de novo
        inventario[i].ocupado = 0; 
        return;
    }
    limpar_buffer_teclado(); // Limpa o '\n' deixado pelo scanf

    // Marca o slot como ocupado e incrementa o contador geral
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
    formatar_entrada(nome_remover);

    int encontrado = 0;
    
    // Busca linear pelo item
    for (int i = 0; i < CAPACIDADE_MAXIMA; i++) {
        // Verifica se a posição está ocupada E se o nome coincide
        if (inventario[i].ocupado == 1 && strcmp(inventario[i].nome, nome_remover) == 0) {
            // Removendo: Apenas marca a posição como livre
            inventario[i].ocupado = 0; 
            (*total_itens)--;
            encontrado = 1;
            printf("\n🗑️ Item '%s' removido com sucesso.\n", nome_remover);
            break; // Termina a busca
        }
    }

    if (!encontrado) {
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
    
    // Itera por toda a capacidade (10), mas só exibe os ocupados
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
    // 💡 Vetor Estático de 10 structs Item
    struct Item mochila[CAPACIDADE_MAXIMA]; 
    
    // 💡 Variável para controlar quantos itens estão ativos (simplicidade)
    int total_itens = 0;

    // Inicializa a mochila:
    // Garante que todos os slots estejam marcados como vazios (ocupado=0)
    for (int i = 0; i < CAPACIDADE_MAXIMA; i++) {
        mochila[i].ocupado = 0;
    }
    
    int opcao;
    
    // 💡 Menu Interativo com do-while
    do {
        listar_itens(mochila, total_itens);
        exibir_menu();
        
        // Lê a opção do menu
        if (scanf("%d", &opcao) != 1) {
            opcao = -1; // Opção inválida
        }
        limpar_buffer_teclado(); // Limpa o '\n' após o scanf

        // 💡 Estrutura de controle switch
        switch (opcao) {
            case 1:
                adicionar_item(mochila, &total_itens);
                break;
            case 2:
                remover_item(mochila, &total_itens);
                break;
            case 3:
                // Já listado no início do loop, mas podemos reforçar
                printf("\nReexibindo a lista...\n"); 
                break;
            case 0:
                printf("\nSaindo do inventário. Até logo!\n");
                break;
            default:
                printf("\n⚠️ Opção inválida. Tente novamente (0-3).\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}