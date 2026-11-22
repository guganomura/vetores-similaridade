#include "array.h"

#include <float.h>
#include <math.h>
#include <stdlib.h>

int inicializar_array(Array *array, int capacidade) {
    if (array == NULL) return 0;
    array->dados = NULL;
    array->tamanho = 0;
    array->capacidade = 0;
    if (capacidade <= 0) return 0;

    /* Um unico bloco contiguo: dados[i] e acessado em O(1). */
    array->dados = malloc((size_t)capacidade * sizeof(float));
    if (array->dados == NULL) return 0;
    array->capacidade = capacidade;
    return 1;
}

int inserir(Array *array, int indice, float valor) {
    int i;
    if (array->tamanho >= array->capacidade) return 0;
    if (indice < 0 || indice > array->tamanho) return 0;

    /* De tras para frente, para nao sobrescrever quem ainda sera movido. */
    for (i = array->tamanho; i > indice; i--) {
        array->dados[i] = array->dados[i - 1];
    }
    array->dados[indice] = valor;
    array->tamanho++;
    return 1;
}

void imprimir(const Array *array, FILE *saida) {
    int i;
    fprintf(saida, "[");
    for (i = 0; i < array->tamanho; i++) {
        if (i > 0) fprintf(saida, ", ");
        fprintf(saida, "%.4f", array->dados[i]);
    }
    fprintf(saida, "]");
}

/* Floats acumulam arredondamento (0.1 + 0.2), entao == nao basta. A tolerancia
 * e relativa para que numeros muito pequenos nao sejam confundidos com zero. */
static int quase_iguais(float a, float b) {
    double maior = fmax(fabs(a), fabs(b));
    return fabs((double)a - b) <= 4 * FLT_EPSILON * maior;
}

int buscar(const Array *array, float valor) {
    int i;
    for (i = 0; i < array->tamanho; i++) {
        if (quase_iguais(array->dados[i], valor)) return i;
    }
    return -1;
}

int remover(Array *array, int indice, float *removido) {
    int i;
    if (indice < 0 || indice >= array->tamanho) return 0;

    if (removido != NULL) *removido = array->dados[indice];
    for (i = indice; i < array->tamanho - 1; i++) {
        array->dados[i] = array->dados[i + 1];
    }
    array->tamanho--;
    return 1;
}

void liberar_array(Array *array) {
    free(array->dados);
    array->dados = NULL;
    array->tamanho = 0;
    array->capacidade = 0;
}
