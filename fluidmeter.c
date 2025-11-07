#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void medirVazao();
void corrigirVolume();
void acessoHistorico();
void registrarCSV(const char *operacao, double valor1, double valor2, double resultado, const char *unidade);
void limpaBuffer();


int main() {
    int opcao;

    while (1) {
        printf("\n=== Sistema de Medição de Fluídos ===\n");
        printf("1. Medição de vazão volumétrica\n");
        printf("2. Correção de volume por temperatura\n");
        printf("3. Acessar histórico (CSV)\n");
        printf("4. Sair\n");
        printf("\nEscolha uma opção: ");

        if (scanf("%d", &opcao) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Opção inválida\n");
            continue;
        }

        switch (opcao) {
            case 1: medirVazao(); break;
            case 2: corrigirVolume(); break;
            case 3: acessoHistorico(); break;
            case 4: printf("Encerrando programa...\n"); return 0;
            default: printf("Opção inválida.\n"); break;
        }
    }

    return 0;
}


// FUNÇÕES

void medirVazao() {
    double area, velocidade;
    printf("\n=== Medição de Vazão Volumétrica ===\n");
    printf("Informe a área da tubulação em m²: ");
    scanf("%lf", &area);
    printf("Informe a velocidade do fluido em m/s: ");
    scanf("%lf", &velocidade);

    double vazao = area * velocidade;
    printf("\nVazão Volumétrica: %.3f m³/s\n", vazao);

    registrarCSV("flow", area, velocidade, vazao, "m³/s");
}

void corrigirVolume() {
    double Vm, alpha, Tm, Tref;
    printf("\n=== Correção de Volume por Temperatura ===\n");
    limpaBuffer();
    printf("Volume medido em m³: ");
    scanf("%lf", &Vm);
    limpaBuffer();
    printf("Coeficiente de expansão 1/°C: ");
    scanf("%lf", &alpha);
    printf("Temperatura medida em °C: ");
    scanf("%lf", &Tm);
    printf("Temperatura de referência em °C: ");
    scanf("%lf", &Tref);

    double Vc = Vm * (1 - alpha * (Tm - Tref));
    printf("Volume corrigido: %.3f m³\n", Vc);

    registrarCSV("temp", Vm, alpha, Vc, "m³");
}

void acessoHistorico() {
    FILE *file = fopen("historico.csv", "r");
    if (!file) {
        printf("Nenhum histórico encontrado.");
        return;
    }

    char linha[256];
    printf("\n=== Histórico de medições ===\n");
    while (fgets(linha, sizeof(linha), file)) {
        printf("%s", linha);
    }
    fclose(file);
}

// FUNÇÕES AUXILIARES

void registrarCSV(const char *operacao, double valor1, double valor2, double resultado, const char *unidade) {
    FILE *file = fopen("historico.csv", "a");
    if (!file) {
        printf("Erro ao tentar abrir o arquivo csv.\n");
        return;
    }

    time_t agora = time(NULL);
    struct tm *t = localtime(&agora);

    fprintf(file, "%02d/%02d/%04d,%s,%.4f,%.4f,%s\n", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900, operacao, valor1, valor2, resultado, unidade);

    fclose(file);
    printf("Registro salvo no historico.");
}

void limpaBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}