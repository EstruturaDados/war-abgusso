#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <time.h>   
#include <locale.h> 

// Constantes Globais
#define MAX_TERRITORIOS 5
#define MAX_MISSOES 3
#define TAM_NOME 30
#define TAM_COR 10

// Estrutura de Dados
typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
} Territorio;

typedef struct {
    int id;
    char descricao[100];
} Missao;

// Protótipos das Funções
Territorio* alocarMapa(int n);
void inicializarTerritorios(Territorio *mapa, int n); // Modifica o mapa (referência)
void liberarMemoria(Territorio *mapa);

// Funções de interface com o usuário:
void exibirMenuPrincipal();
void exibirMapa(const Territorio *mapa, int n); // Apenas lê o mapa (referência constante)
void exibirMissao(const Missao *missao);        // Apenas lê a missão

// Funções de lógica principal do jogo:
void faseDeAtaque(Territorio *mapa, int n); // Modifica o mapa (referência)
void simularAtaque(Territorio *origem, Territorio *destino); // Modifica territórios (referência)
Missao sortearMissao();
int verificarVitoria(const Territorio *mapa, int n, const Missao *missao); // Apenas lê

// Função utilitária:
void limparBufferEntrada();

// --- Função Principal (main) ---
int main() {
    // Variáveis de controle do jogo
    Territorio *mapa = NULL;
    Missao missao_jogador;
    int escolha = -1;
    int vitoria = 0;
    
    // 1. Configuração Inicial (Setup):
    setlocale(LC_ALL, "Portuguese"); // Define o locale para português
    srand((unsigned int)time(NULL)); // Inicializa a semente para geração de números aleatórios

    printf("--- WAR Estruturado (Nível Intermediário) ---\n");
    
    // Aloca a memória para o mapa e verifica
    mapa = alocarMapa(MAX_TERRITORIOS);
    if (mapa == NULL) {
        printf("Erro ao alocar memória para o mapa. Encerrando.\n");
        return 1;
    }
    
    // Preenche os territórios com dados iniciais
    inicializarTerritorios(mapa, MAX_TERRITORIOS);
    
    // Sorteia a missão secreta
    missao_jogador = sortearMissao();
    printf("Configuração inicial concluída. Boa sorte!\n");

// ---
    
    // 2. Laço Principal do Jogo (Game Loop):
    do {
        printf("\n========================================\n");
        exibirMapa(mapa, MAX_TERRITORIOS);
        exibirMissao(&missao_jogador);
        exibirMenuPrincipal();
        
        printf("Escolha sua ação (0-2): ");
        if (scanf("%d", &escolha) != 1) {
            escolha = -1; // Sinaliza uma entrada inválida
            limparBufferEntrada();
        }

        switch (escolha) {
            case 1:
                printf("\n--- FASE DE ATAQUE ---\n");
                faseDeAtaque(mapa, MAX_TERRITORIOS);
                break;
            case 2:
                vitoria = verificarVitoria(mapa, MAX_TERRITORIOS, &missao_jogador);
                if (vitoria) {
                    printf("\n🎉 PARABÉNS! VOCÊ CUMPRIU SUA MISSÃO SECRETA E VENCEU O JOGO! 🎉\n");
                    escolha = 0; // Encerra o jogo
                } else {
                    printf("\nA missão ainda NÃO foi cumprida. Continue atacando!\n");
                }
                break;
            case 0:
                printf("\nEncerrando o jogo...\n");
                break;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
                break;
        }

    } while (escolha != 0);

// ---

    // 3. Limpeza:
    liberarMemoria(mapa);
    
    return 0;
}

// --- Implementação das Funções ---

// Alocação Dinâmica e Inicialização

Territorio* alocarMapa(int n) {
    // Aloca dinamicamente 'n' estruturas do tipo Territorio e inicializa com zeros.
    Territorio *mapa = (Territorio *)calloc(n, sizeof(Territorio));
    return mapa;
}

void inicializarTerritorios(Territorio *mapa, int n) {
    // Usa passagem por referência (ponteiro) para preencher os dados no mapa alocado.
    if (mapa == NULL) return;

    // Inicialização simples para demonstração (você pode expandir isso)
    if (n > 0) {
        strcpy(mapa[0].nome, "Brasil");
        strcpy(mapa[0].cor, "Azul");
        mapa[0].tropas = 5;

        strcpy(mapa[1].nome, "Chile");
        strcpy(mapa[1].cor, "Vermelho");
        mapa[1].tropas = 3;
        
        // ... inicializar o restante ...
    }
    printf("Territórios iniciais definidos.\n");
}

void liberarMemoria(Territorio *mapa) {
    // Libera a memória previamente alocada.
    if (mapa != NULL) {
        free(mapa);
        printf("\nMemória do mapa liberada com sucesso.\n");
    }
}

// Interface com o Usuário

void exibirMenuPrincipal() {
    printf("\nMENU DE AÇÕES:\n");
    printf(" [1] - Iniciar Fase de Ataque\n");
    printf(" [2] - Verificar Vitória (Missão)\n");
    printf(" [0] - Sair do Jogo\n");
}

void exibirMapa(const Territorio *mapa, int n) {
    // Usa 'const' para garantir que a função apenas leia os dados.
    printf("\n--- Mapa Atual ---\n");
    printf("| %-20s | %-10s | %-7s |\n", "TERRITÓRIO", "DONO", "TROPAS");
    printf("|----------------------|------------|---------|\n");
    for (int i = 0; i < n; i++) {
        // Assume que só exibimos os 2 primeiros que foram inicializados no exemplo.
        if (mapa[i].tropas > 0) { 
            printf("| %-20s | %-10s | %-7d |\n", mapa[i].nome, mapa[i].cor, mapa[i].tropas);
        }
    }
    printf("--------------------------------------\n");
}

void exibirMissao(const Missao *missao) {
    // Apenas lê a struct Missao.
    printf("\nMISSAO SECRETA:\n");
    printf("ID %d: %s\n", missao->id, missao->descricao);
}

// Lógica Principal do Jogo

Missao sortearMissao() {
    Missao missoes[MAX_MISSOES] = {
        {1, "Conquistar 18 Territórios no total."},
        {2, "Destruir completamente o exército Vermelho."},
        {3, "Conquistar 24 Territórios no total."}
    };
    
    // Sorteia um índice de 0 a MAX_MISSOES - 1
    int indice = rand() % MAX_MISSOES;
    printf("Missão sorteada.\n");
    return missoes[indice];
}

void faseDeAtaque(Territorio *mapa, int n) {
    // Esta função precisaria de lógica complexa de busca e validação,
    // mas aqui apenas simulamos a chamada para simularAtaque.
    
    // OBS: Implementação real exigiria que o jogador escolhesse 
    // um território de 'origem' e um de 'destino' válido.
    
    printf("Ataque iniciado! (Simulação de ataque entre Brasil e Chile)\n");
    
    // Exemplo de como usar ponteiros para passar as structs a serem modificadas.
    // Buscamos os ponteiros para os territórios no mapa.
    Territorio *origem = &mapa[0]; // Brasil (Azul)
    Territorio *destino = &mapa[1]; // Chile (Vermelho)
    
    if (origem->tropas > 1) { // Verifica se há tropas suficientes
        simularAtaque(origem, destino);
    } else {
        printf("Tropas insuficientes para atacar.\n");
    }
}

void simularAtaque(Territorio *origem, Territorio *destino) {
    // Implementação esquelética. No jogo real:
    // 1. Rolar dados do atacante (origem->tropas - 1) e defensor (destino->tropas).
    // 2. Comparar resultados e calcular perdas.
    // 3. Atualizar as tropas de ambos.
    
    printf("Simulando batalha...\n");
    
    // Exemplo: Atacante perde 1, Defensor perde 1
    origem->tropas -= 1;
    destino->tropas -= 1;
    
    // Lógica de conquista:
    if (destino->tropas <= 0) {
        printf("Território %s CONQUISTADO!\n", destino->nome);
        // Atualiza o dono
        strcpy(destino->cor, origem->cor);
        // Move uma tropa
        origem->tropas -= 1;
        destino->tropas += 1;
    }
}

int verificarVitoria(const Territorio *mapa, int n, const Missao *missao) {
    // Implementa a lógica para cada tipo de missão
    if (missao->id == 1) {
        // Missão 1: Conquistar 18 Territórios (lógica: contar quantos territórios pertencem ao jogador)
        int territorios_jogador = 0;
        char cor_jogador[TAM_COR] = "Azul"; // Assumindo 'Azul' como cor do jogador no setup
        
        for (int i = 0; i < n; i++) {
            if (strcmp(mapa[i].cor, cor_jogador) == 0) {
                territorios_jogador++;
            }
        }
        
        if (territorios_jogador >= 18) { // Se o total for maior ou igual a 18
            return 1;
        }
    } 
    // Outras missões seriam implementadas aqui...
    
    return 0; // Missão não cumprida
}

// Utilitário

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Limpa o buffer de entrada (stdin)
    }
}