#ifndef ARRAY_H
#define ARRAY_H

#include <stdio.h>

/*
 * Array de numeros reais com capacidade fixa, definida na inicializacao.
 * Os elementos ocupam as posicoes 0 a tamanho - 1, sem buracos.
 */
typedef struct {
    float *dados;
    int tamanho;    /* posicoes ocupadas */
    int capacidade; /* posicoes alocadas */
} Array;

/* Retorna 1 em caso de sucesso e 0 em caso de erro. */
int inicializar_array(Array *array, int capacidade);

/* Insere no indice, deslocando os seguintes para a direita. Retorna 1 ou 0. */
int inserir(Array *array, int indice, float valor);

/* Escreve o array no formato [1.0000, 2.0000]. */
void imprimir(const Array *array, FILE *saida);

/* Retorna o indice da primeira ocorrencia do valor, ou -1. */
int buscar(const Array *array, float valor);

/* Remove do indice, deslocando os seguintes para a esquerda. Retorna 1 ou 0.
 * Se removido nao for NULL, recebe o valor que saiu. */
int remover(Array *array, int indice, float *removido);

/* Auxiliar exigida pelo C para devolver a memoria alocada. */
void liberar_array(Array *array);

#endif
