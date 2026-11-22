#ifndef OPERACOES_H
#define OPERACOES_H

#include "array.h"

typedef enum {
    OK,
    ERRO_VAZIO,
    ERRO_DIMENSAO,
    ERRO_VETOR_NULO,
    ERRO_VALOR,
    ERRO_MEMORIA,
    ERRO_SEM_CANDIDATO
} Status;

const char *mensagem_status(Status status);

/*
 * O vetor resultado e inicializado pela funcao, deve ser diferente das entradas
 * e deve ser liberado por quem chamou. Em caso de erro, nada fica alocado.
 */
Status multiplicar_escalar(const Array *a, float k, Array *resultado);
Status somar(const Array *a, const Array *b, Array *resultado);

Status produto_escalar(const Array *a, const Array *b, double *resultado);
Status norma(const Array *a, double *resultado);
Status similaridade_cosseno(const Array *a, const Array *b, double *resultado);

/*
 * Ignora a propria consulta (se estiver no conjunto) e os vetores vazios,
 * nulos ou de outra dimensao. Em empate, vence o de menor indice.
 */
Status mais_similar(const Array *consulta, const Array vetores[], int quantidade,
                    int *indice, double *similaridade);

#endif
