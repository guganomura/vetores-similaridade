/* Interface de terminal. As mensagens nao usam acentos para aparecerem
 * corretamente tambem no terminal do Windows. */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "operacoes.h"

#define MAX_VETORES 100
#define MAX_DIMENSAO 10000
#define TAM_LINHA 128

/* Os vetores tambem ficam em um array: vetores[0] e exibido como V1. */
static Array vetores[MAX_VETORES];
static int quantidade = 0;

static void liberar_tudo(void) {
    int i;
    for (i = 0; i < quantidade; i++) liberar_array(&vetores[i]);
    quantidade = 0;
}

/* ---------- Entrada ---------- */

static void ler_linha(const char *pergunta, char linha[TAM_LINHA]) {
    int c;
    for (;;) {
        printf("%s", pergunta);
        fflush(stdout);
        if (fgets(linha, TAM_LINHA, stdin) == NULL) {
            printf("\nPrograma encerrado.\n");
            exit(0);
        }
        if (strchr(linha, '\n') != NULL || feof(stdin)) break;

        while ((c = getchar()) != '\n' && c != EOF) {
            /* descarta o restante da linha */
        }
        printf("Entrada muito longa.\n");
    }
    linha[strcspn(linha, "\r\n")] = '\0';
}

static int ler_inteiro(const char *pergunta, int minimo, int maximo) {
    char linha[TAM_LINHA], *fim;
    long valor;
    for (;;) {
        ler_linha(pergunta, linha);
        valor = strtol(linha, &fim, 10);
        if (fim != linha && *fim == '\0' && valor >= minimo && valor <= maximo)
            return (int)valor;
        printf("Digite um numero inteiro entre %d e %d.\n", minimo, maximo);
    }
}

static float ler_real(const char *pergunta) {
    char linha[TAM_LINHA], *fim, *virgula;
    float valor;
    for (;;) {
        ler_linha(pergunta, linha);
        if ((virgula = strchr(linha, ',')) != NULL) *virgula = '.';
        valor = strtof(linha, &fim);
        if (fim != linha && *fim == '\0' && isfinite(valor)) return valor;
        printf("Digite um numero real valido.\n");
    }
}

static int confirmar(const char *pergunta) {
    char linha[TAM_LINHA];
    for (;;) {
        ler_linha(pergunta, linha);
        if (strcmp(linha, "s") == 0 || strcmp(linha, "S") == 0) return 1;
        if (strcmp(linha, "n") == 0 || strcmp(linha, "N") == 0) return 0;
        printf("Responda com s ou n.\n");
    }
}

static Array *escolher_vetor(const char *pergunta) {
    return &vetores[ler_inteiro(pergunta, 1, quantidade) - 1];
}

/* ---------- Saida ---------- */

static void mostrar_erro(Status status) {
    printf("Operacao invalida: %s.\n", mensagem_status(status));
}

static void mostrar_vetor(const Array *vetor) {
    printf("V%d (dimensao %d): ", (int)(vetor - vetores) + 1, vetor->tamanho);
    imprimir(vetor, stdout);
    printf("\n");
}

/* Mostra o vetor calculado e o guarda ou libera. */
static void concluir_resultado(Array *resultado) {
    printf("Resultado: ");
    imprimir(resultado, stdout);
    printf("\n");

    if (quantidade == MAX_VETORES) {
        printf("Limite de vetores atingido, resultado nao salvo.\n");
        liberar_array(resultado);
    } else if (confirmar("Salvar como novo vetor? (s/n): ")) {
        vetores[quantidade++] = *resultado;
        printf("Resultado salvo como V%d.\n", quantidade);
    } else {
        liberar_array(resultado);
    }
}

/* ---------- Opcoes do menu ---------- */

static void criar_vetores(void) {
    char pergunta[64];
    int n, i, j, tamanho;

    if (quantidade == MAX_VETORES) {
        printf("Limite de %d vetores atingido.\n", MAX_VETORES);
        return;
    }
    n = ler_inteiro("Quantos vetores deseja criar? ", 1, MAX_VETORES - quantidade);

    for (i = 0; i < n; i++) {
        Array *novo = &vetores[quantidade];

        snprintf(pergunta, sizeof pergunta, "Tamanho de V%d: ", quantidade + 1);
        tamanho = ler_inteiro(pergunta, 1, MAX_DIMENSAO);
        if (!inicializar_array(novo, tamanho)) {
            mostrar_erro(ERRO_MEMORIA);
            return;
        }
        quantidade++; /* antes da leitura, para ser liberado se a entrada acabar */

        for (j = 0; j < tamanho; j++) {
            snprintf(pergunta, sizeof pergunta, "  V%d[%d] = ", quantidade, j);
            inserir(novo, j, ler_real(pergunta));
        }
        printf("V%d criado (dimensao %d).\n", quantidade, tamanho);
    }
}

static void exibir_vetores(void) {
    int i;
    for (i = 0; i < quantidade; i++) mostrar_vetor(&vetores[i]);
}

static void opcao_multiplicar(void) {
    Array resultado;
    const Array *a = escolher_vetor("Numero do vetor: ");
    float k = ler_real("Escalar k: ");
    Status status = multiplicar_escalar(a, k, &resultado);

    if (status == OK) concluir_resultado(&resultado);
    else mostrar_erro(status);
}

static void opcao_somar(void) {
    Array resultado;
    const Array *a = escolher_vetor("Primeiro vetor: ");
    const Array *b = escolher_vetor("Segundo vetor: ");
    Status status = somar(a, b, &resultado);

    if (status == OK) concluir_resultado(&resultado);
    else mostrar_erro(status);
}

static void opcao_produto(void) {
    double resultado;
    const Array *a = escolher_vetor("Primeiro vetor: ");
    const Array *b = escolher_vetor("Segundo vetor: ");
    Status status = produto_escalar(a, b, &resultado);

    if (status == OK) printf("Produto escalar: %.4f\n", resultado);
    else mostrar_erro(status);
}

static void opcao_norma(void) {
    double resultado;
    const Array *a = escolher_vetor("Numero do vetor: ");
    Status status = norma(a, &resultado);

    if (status == OK) printf("Norma: %.4f\n", resultado);
    else mostrar_erro(status);
}

static void opcao_similaridade(void) {
    double resultado;
    const Array *a = escolher_vetor("Primeiro vetor: ");
    const Array *b = escolher_vetor("Segundo vetor: ");
    Status status = similaridade_cosseno(a, b, &resultado);

    if (status == OK) printf("Similaridade: %.4f\n", resultado);
    else mostrar_erro(status);
}

static void opcao_mais_similar(void) {
    double similaridade;
    int indice;
    const Array *consulta = escolher_vetor("Vetor de consulta: ");
    Status status = mais_similar(consulta, vetores, quantidade, &indice, &similaridade);

    if (status != OK) {
        mostrar_erro(status);
        return;
    }
    printf("Mais similar a V%d: V%d (similaridade %.4f)\n",
           (int)(consulta - vetores) + 1, indice + 1, similaridade);
}

static void opcao_buscar(void) {
    const Array *a = escolher_vetor("Numero do vetor: ");
    int indice = buscar(a, ler_real("Valor procurado: "));

    if (indice >= 0) printf("Valor encontrado no indice %d.\n", indice);
    else printf("Valor nao encontrado.\n");
}

static void opcao_inserir(void) {
    Array *a = escolher_vetor("Numero do vetor: ");
    int indice;

    if (a->tamanho == a->capacidade) {
        printf("Operacao invalida: array cheio (remova um elemento antes).\n");
        return;
    }
    indice = ler_inteiro("Indice de insercao: ", 0, a->tamanho);
    inserir(a, indice, ler_real("Valor: "));
    mostrar_vetor(a);
}

static void opcao_remover(void) {
    Array *a = escolher_vetor("Numero do vetor: ");
    float removido;

    if (a->tamanho == 0) {
        printf("Operacao invalida: array vazio.\n");
        return;
    }
    if (remover(a, ler_inteiro("Indice de remocao: ", 0, a->tamanho - 1), &removido)) {
        printf("Valor %.4f removido.\n", removido);
        mostrar_vetor(a);
    }
}

static void mostrar_menu(void) {
    printf("\n=== VETORES E SIMILARIDADE DE COSSENO ===\n"
           " 1. Criar vetores\n"
           " 2. Exibir vetores\n"
           " 3. Multiplicar por escalar\n"
           " 4. Somar dois vetores\n"
           " 5. Produto escalar\n"
           " 6. Norma\n"
           " 7. Similaridade de cosseno\n"
           " 8. Encontrar o mais similar\n"
           " 9. Buscar valor em um vetor\n"
           "10. Inserir valor em um vetor\n"
           "11. Remover valor de um vetor\n"
           " 0. Sair\n");
}

int main(void) {
    int opcao;
    atexit(liberar_tudo);

    for (;;) {
        mostrar_menu();
        opcao = ler_inteiro("Opcao: ", 0, 11);

        if (opcao == 0) break;
        if (opcao != 1 && quantidade == 0) {
            printf("Nenhum vetor armazenado. Use a opcao 1.\n");
            continue;
        }
        switch (opcao) {
            case 1: criar_vetores(); break;
            case 2: exibir_vetores(); break;
            case 3: opcao_multiplicar(); break;
            case 4: opcao_somar(); break;
            case 5: opcao_produto(); break;
            case 6: opcao_norma(); break;
            case 7: opcao_similaridade(); break;
            case 8: opcao_mais_similar(); break;
            case 9: opcao_buscar(); break;
            case 10: opcao_inserir(); break;
            case 11: opcao_remover(); break;
        }
    }
    printf("Programa encerrado.\n");
    return 0;
}
