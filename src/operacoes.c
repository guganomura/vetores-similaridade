#include "operacoes.h"

#include <math.h>

const char *mensagem_status(Status status) {
    switch (status) {
        case OK: return "sucesso";
        case ERRO_VAZIO: return "vetor vazio";
        case ERRO_DIMENSAO: return "os vetores possuem dimensoes diferentes";
        case ERRO_VETOR_NULO: return "vetor nulo nao possui similaridade de cosseno";
        case ERRO_VALOR: return "o resultado excede o limite de um float";
        case ERRO_MEMORIA: return "memoria insuficiente";
        case ERRO_SEM_CANDIDATO: return "nenhum outro vetor pode ser comparado com a consulta";
    }
    return "erro desconhecido";
}

static Status validar_par(const Array *a, const Array *b) {
    if (a->tamanho == 0 || b->tamanho == 0) return ERRO_VAZIO;
    if (a->tamanho != b->tamanho) return ERRO_DIMENSAO;
    return OK;
}

/* B[i] = k * A[i] */
Status multiplicar_escalar(const Array *a, float k, Array *resultado) {
    int i;
    if (a->tamanho == 0) return ERRO_VAZIO;
    if (!inicializar_array(resultado, a->tamanho)) return ERRO_MEMORIA;

    for (i = 0; i < a->tamanho; i++) {
        float valor = k * a->dados[i];
        if (!isfinite(valor)) {
            liberar_array(resultado);
            return ERRO_VALOR;
        }
        inserir(resultado, i, valor);
    }
    return OK;
}

/* C[i] = A[i] + B[i] */
Status somar(const Array *a, const Array *b, Array *resultado) {
    int i;
    Status status = validar_par(a, b);
    if (status != OK) return status;
    if (!inicializar_array(resultado, a->tamanho)) return ERRO_MEMORIA;

    for (i = 0; i < a->tamanho; i++) {
        float valor = a->dados[i] + b->dados[i];
        if (!isfinite(valor)) {
            liberar_array(resultado);
            return ERRO_VALOR;
        }
        inserir(resultado, i, valor);
    }
    return OK;
}

/* A . B = soma de A[i] * B[i]. O acumulador e double para nao estourar. */
Status produto_escalar(const Array *a, const Array *b, double *resultado) {
    int i;
    double soma = 0.0;
    Status status = validar_par(a, b);
    if (status != OK) return status;

    for (i = 0; i < a->tamanho; i++) {
        soma += (double)a->dados[i] * b->dados[i];
    }
    *resultado = soma;
    return OK;
}

/* ||A|| = raiz da soma de A[i]^2 */
Status norma(const Array *a, double *resultado) {
    int i;
    double soma = 0.0;
    if (a->tamanho == 0) return ERRO_VAZIO;

    for (i = 0; i < a->tamanho; i++) {
        soma += (double)a->dados[i] * a->dados[i];
    }
    *resultado = sqrt(soma);
    return OK;
}

/* cos(A, B) = (A . B) / (||A|| * ||B||) */
Status similaridade_cosseno(const Array *a, const Array *b, double *resultado) {
    double produto, norma_a, norma_b, cosseno;
    Status status = produto_escalar(a, b, &produto);
    if (status != OK) return status;
    if ((status = norma(a, &norma_a)) != OK) return status;
    if ((status = norma(b, &norma_b)) != OK) return status;
    if (norma_a == 0.0 || norma_b == 0.0) return ERRO_VETOR_NULO;

    cosseno = produto / (norma_a * norma_b);
    /* O arredondamento pode gerar algo como 1.0000000000000002. */
    *resultado = fmax(-1.0, fmin(1.0, cosseno));
    return OK;
}

Status mais_similar(const Array *consulta, const Array vetores[], int quantidade,
                    int *indice, double *similaridade) {
    int i, melhor = -1;
    double atual, maior = 0.0, norma_consulta;
    Status status = norma(consulta, &norma_consulta);
    if (status != OK) return status;
    if (norma_consulta == 0.0) return ERRO_VETOR_NULO;

    for (i = 0; i < quantidade; i++) {
        if (&vetores[i] == consulta) continue;
        if (similaridade_cosseno(consulta, &vetores[i], &atual) != OK) continue;
        if (melhor == -1 || atual > maior) {
            melhor = i;
            maior = atual;
        }
    }
    if (melhor == -1) return ERRO_SEM_CANDIDATO;

    *indice = melhor;
    *similaridade = maior;
    return OK;
}
