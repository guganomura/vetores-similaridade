#include <float.h>
#include <math.h>
#include <string.h>

#include "../src/array.h"
#include "../src/operacoes.h"

static int total, falhas;

static void verificar(const char *id, const char *descricao, int condicao) {
    total++;
    if (!condicao) falhas++;
    printf("[%s] %s %s\n", condicao ? "OK" : "FALHA", id, descricao);
}

static int perto(double a, double b) {
    return fabs(a - b) < 1e-5;
}

static Array criar(const float *valores, int n) {
    Array a;
    int i;
    if (inicializar_array(&a, n))
        for (i = 0; i < n; i++) inserir(&a, i, valores[i]);
    return a;
}

static int igual(const Array *a, const float *esperado, int n) {
    int i;
    if (a->tamanho != n) return 0;
    for (i = 0; i < n; i++)
        if (!perto(a->dados[i], esperado[i])) return 0;
    return 1;
}

static int impresso(const Array *a, const char *esperado) {
    char texto[128] = "";
    FILE *f = tmpfile();
    size_t n;
    if (f == NULL) return 0;
    imprimir(a, f);
    rewind(f);
    n = fread(texto, 1, sizeof texto - 1, f);
    texto[n] = '\0';
    fclose(f);
    return strcmp(texto, esperado) == 0;
}

static void testar_inicializar_array(void) {
    Array a;
    verificar("INI-01", "cria array vazio com capacidade 3",
              inicializar_array(&a, 3) && a.dados && a.tamanho == 0 && a.capacidade == 3);
    liberar_array(&a);
    verificar("INI-02", "rejeita capacidade zero", !inicializar_array(&a, 0) && !a.dados);
    verificar("INI-03", "rejeita capacidade negativa", !inicializar_array(&a, -1) && !a.dados);
    verificar("INI-04", "rejeita ponteiro NULL", !inicializar_array(NULL, 3));
    inicializar_array(&a, 2);
    liberar_array(&a);
    liberar_array(&a);
    verificar("INI-05", "liberar duas vezes deixa o array zerado",
              !a.dados && a.tamanho == 0 && a.capacidade == 0);
}

static void testar_inserir(void) {
    const float esperado[] = {1, 2, 3, 4};
    Array a;
    inicializar_array(&a, 4);
    verificar("INS-01", "insere em array vazio", inserir(&a, 0, 2) && a.dados[0] == 2);
    verificar("INS-02", "insere no fim", inserir(&a, 1, 4) && a.dados[1] == 4);
    verificar("INS-03", "insere no inicio deslocando", inserir(&a, 0, 1) && a.dados[1] == 2);
    verificar("INS-04", "rejeita indice negativo", !inserir(&a, -1, 9) && a.tamanho == 3);
    verificar("INS-05", "rejeita indice que deixaria buraco", !inserir(&a, 4, 9) && a.tamanho == 3);
    verificar("INS-06", "insere no meio deslocando", inserir(&a, 2, 3) && igual(&a, esperado, 4));
    verificar("INS-07", "rejeita array cheio", !inserir(&a, 0, 9) && igual(&a, esperado, 4));
    liberar_array(&a);
    verificar("INS-08", "rejeita array nao alocado", !inserir(&a, 0, 1));
}

static void testar_imprimir(void) {
    const float v[] = {1, -2.5f, 0}, um[] = {7};
    Array a = criar(v, 3), b = criar(um, 1), vazio;
    inicializar_array(&vazio, 2);
    verificar("IMP-01", "imprime varios valores", impresso(&a, "[1.0000, -2.5000, 0.0000]"));
    verificar("IMP-02", "imprime um valor", impresso(&b, "[7.0000]"));
    verificar("IMP-03", "imprime array vazio", impresso(&vazio, "[]"));
    liberar_array(&a);
    liberar_array(&b);
    liberar_array(&vazio);
}

static void testar_buscar(void) {
    const float v[] = {5, -2, 5, 0.3f, 9}, zero[] = {0};
    Array a = criar(v, 5), z = criar(zero, 1), vazio;
    inicializar_array(&vazio, 2);
    verificar("BUS-01", "encontra no inicio e retorna a primeira ocorrencia", buscar(&a, 5) == 0);
    verificar("BUS-02", "encontra valor negativo", buscar(&a, -2) == 1);
    verificar("BUS-03", "encontra no fim", buscar(&a, 9) == 4);
    verificar("BUS-04", "retorna -1 se ausente", buscar(&a, 8) == -1);
    verificar("BUS-05", "tolera arredondamento de float",
              buscar(&a, nextafterf(0.3f, 1.0f)) == 3);
    verificar("BUS-06", "nao confunde valor pequeno com zero", buscar(&z, 1e-8f) == -1);
    verificar("BUS-07", "retorna -1 em array vazio", buscar(&vazio, 0) == -1);
    liberar_array(&a);
    liberar_array(&z);
    liberar_array(&vazio);
}

static void testar_remover(void) {
    const float v[] = {1, 2, 3, 4}, meio[] = {2, 4};
    Array a = criar(v, 4);
    float x = 0;
    verificar("REM-01", "remove do inicio e devolve o valor",
              remover(&a, 0, &x) && x == 1 && a.dados[0] == 2);
    verificar("REM-02", "remove do meio deslocando", remover(&a, 1, &x) && igual(&a, meio, 2));
    verificar("REM-03", "rejeita indice negativo", !remover(&a, -1, &x) && a.tamanho == 2);
    verificar("REM-04", "rejeita indice igual ao tamanho", !remover(&a, 2, &x) && a.tamanho == 2);
    verificar("REM-05", "remove do fim", remover(&a, 1, &x) && x == 4 && a.tamanho == 1);
    verificar("REM-06", "aceita destino NULL", remover(&a, 0, NULL) && a.tamanho == 0);
    verificar("REM-07", "rejeita array vazio", !remover(&a, 0, &x));
    verificar("REM-08", "permite inserir apos esvaziar", inserir(&a, 0, 8) && a.dados[0] == 8);
    liberar_array(&a);
}

static void testar_multiplicacao(void) {
    const float v[] = {1, -2, 3}, dobro[] = {2, -4, 6}, zero[] = {0, 0, 0};
    const float oposto[] = {-1, 2, -3}, meio[] = {0.5f, -1, 1.5f}, maximo[] = {FLT_MAX};
    Array a = criar(v, 3), m = criar(maximo, 1), vazio, r;
    inicializar_array(&vazio, 1);
    verificar("MUL-01", "escalar positivo", multiplicar_escalar(&a, 2, &r) == OK && igual(&r, dobro, 3));
    liberar_array(&r);
    verificar("MUL-02", "escalar zero", multiplicar_escalar(&a, 0, &r) == OK && igual(&r, zero, 3));
    liberar_array(&r);
    verificar("MUL-03", "escalar negativo", multiplicar_escalar(&a, -1, &r) == OK && igual(&r, oposto, 3));
    liberar_array(&r);
    verificar("MUL-04", "escalar fracionario", multiplicar_escalar(&a, 0.5f, &r) == OK && igual(&r, meio, 3));
    verificar("MUL-05", "nao altera o original", igual(&a, v, 3));
    liberar_array(&r);
    verificar("MUL-06", "rejeita vetor vazio", multiplicar_escalar(&vazio, 2, &r) == ERRO_VAZIO);
    verificar("MUL-07", "rejeita estouro de float",
              multiplicar_escalar(&m, 2, &r) == ERRO_VALOR && !r.dados);
    liberar_array(&a);
    liberar_array(&m);
    liberar_array(&vazio);
}

static void testar_soma(void) {
    const float va[] = {1, 2, 3}, vb[] = {4, -5, 6.5f}, esperado[] = {5, -3, 9.5f};
    const float maximo[] = {FLT_MAX};
    Array a = criar(va, 3), b = criar(vb, 3), c = criar(va, 2), m = criar(maximo, 1), vazio, r;
    inicializar_array(&vazio, 1);
    verificar("SOM-01", "soma posicao a posicao", somar(&a, &b, &r) == OK && igual(&r, esperado, 3));
    verificar("SOM-02", "nao altera as entradas", igual(&a, va, 3) && igual(&b, vb, 3));
    liberar_array(&r);
    verificar("SOM-03", "e comutativa", somar(&b, &a, &r) == OK && igual(&r, esperado, 3));
    liberar_array(&r);
    verificar("SOM-04", "rejeita segundo vetor menor", somar(&a, &c, &r) == ERRO_DIMENSAO && !r.dados);
    verificar("SOM-05", "rejeita primeiro vetor menor", somar(&c, &a, &r) == ERRO_DIMENSAO);
    verificar("SOM-06", "rejeita vetor vazio", somar(&vazio, &vazio, &r) == ERRO_VAZIO);
    verificar("SOM-07", "rejeita estouro de float", somar(&m, &m, &r) == ERRO_VALOR && !r.dados);
    liberar_array(&a);
    liberar_array(&b);
    liberar_array(&c);
    liberar_array(&m);
    liberar_array(&vazio);
}

static void testar_produto(void) {
    const float va[] = {1, 2, 3}, vb[] = {4, 5, 6}, vx[] = {1, 0, 0}, vy[] = {0, -1, 0};
    Array a = criar(va, 3), b = criar(vb, 3), x = criar(vx, 3), y = criar(vy, 3), c = criar(va, 2), vazio;
    double r;
    inicializar_array(&vazio, 1);
    verificar("PRO-01", "exemplo do enunciado = 32", produto_escalar(&a, &b, &r) == OK && perto(r, 32));
    verificar("PRO-02", "ortogonais = 0", produto_escalar(&x, &y, &r) == OK && perto(r, 0));
    verificar("PRO-03", "resultado negativo", produto_escalar(&a, &y, &r) == OK && perto(r, -2));
    verificar("PRO-04", "rejeita dimensoes diferentes", produto_escalar(&a, &c, &r) == ERRO_DIMENSAO);
    verificar("PRO-05", "rejeita vetor vazio", produto_escalar(&vazio, &a, &r) == ERRO_VAZIO);
    liberar_array(&a);
    liberar_array(&b);
    liberar_array(&x);
    liberar_array(&y);
    liberar_array(&c);
    liberar_array(&vazio);
}

static void testar_norma(void) {
    const float va[] = {3, 4}, vb[] = {-3, -4}, vz[] = {0, 0}, grande[] = {FLT_MAX};
    Array a = criar(va, 2), b = criar(vb, 2), z = criar(vz, 2), g = criar(grande, 1), vazio;
    double r;
    inicializar_array(&vazio, 1);
    verificar("NOR-01", "norma de [3, 4] = 5", norma(&a, &r) == OK && perto(r, 5));
    verificar("NOR-02", "componentes negativos", norma(&b, &r) == OK && perto(r, 5));
    verificar("NOR-03", "vetor nulo = 0", norma(&z, &r) == OK && r == 0);
    verificar("NOR-04", "maior float sem estouro", norma(&g, &r) == OK && perto(r / FLT_MAX, 1));
    verificar("NOR-05", "rejeita vetor vazio", norma(&vazio, &r) == ERRO_VAZIO);
    liberar_array(&a);
    liberar_array(&b);
    liberar_array(&z);
    liberar_array(&g);
    liberar_array(&vazio);
}

static void testar_similaridade(void) {
    const float va[] = {0.8f, 0.2f, 0.5f, 0.9f}, vb[] = {0.7f, 0.1f, 0.6f, 0.8f};
    const float vx[] = {1, 0}, vy[] = {0, 1}, vn[] = {-3, 0}, vz[] = {0, 0}, vp[] = {1e-30f, 0};
    Array a = criar(va, 4), b = criar(vb, 4), x = criar(vx, 2), y = criar(vy, 2);
    Array n = criar(vn, 2), z = criar(vz, 2), p = criar(vp, 2), vazio;
    double r;
    inicializar_array(&vazio, 1);
    verificar("COS-01", "vetores do enunciado = 0.990375",
              similaridade_cosseno(&a, &b, &r) == OK && perto(r, 0.990375));
    verificar("COS-02", "e simetrica", similaridade_cosseno(&b, &a, &r) == OK && perto(r, 0.990375));
    verificar("COS-03", "identicos = 1, sem passar de 1",
              similaridade_cosseno(&a, &a, &r) == OK && perto(r, 1) && r <= 1);
    verificar("COS-04", "ortogonais = 0", similaridade_cosseno(&x, &y, &r) == OK && perto(r, 0));
    verificar("COS-05", "opostos = -1", similaridade_cosseno(&x, &n, &r) == OK && perto(r, -1));
    verificar("COS-06", "valor minusculo nao e vetor nulo",
              similaridade_cosseno(&p, &x, &r) == OK && perto(r, 1));
    verificar("COS-07", "rejeita primeiro vetor nulo", similaridade_cosseno(&z, &x, &r) == ERRO_VETOR_NULO);
    verificar("COS-08", "rejeita segundo vetor nulo", similaridade_cosseno(&x, &z, &r) == ERRO_VETOR_NULO);
    verificar("COS-09", "rejeita dimensoes diferentes", similaridade_cosseno(&a, &x, &r) == ERRO_DIMENSAO);
    verificar("COS-10", "rejeita vetor vazio", similaridade_cosseno(&x, &vazio, &r) == ERRO_VAZIO);
    liberar_array(&a);
    liberar_array(&b);
    liberar_array(&x);
    liberar_array(&y);
    liberar_array(&n);
    liberar_array(&z);
    liberar_array(&p);
    liberar_array(&vazio);
}

static void testar_mais_similar(void) {
    const float q[] = {1, 0}, longe[] = {0, 1}, perto_q[] = {2, 1}, oposto[] = {-1, 0};
    const float nulo[] = {0, 0}, outra[] = {1, 0, 0};
    Array v[7];
    double s;
    int i, indice;
    v[0] = criar(q, 2);
    v[1] = criar(nulo, 2);
    v[2] = criar(outra, 3);
    v[3] = criar(oposto, 2);
    v[4] = criar(longe, 2);
    v[5] = criar(perto_q, 2);
    v[6] = criar(perto_q, 2);
    verificar("MAI-01", "escolhe o de maior cosseno",
              mais_similar(&v[0], v, 6, &indice, &s) == OK && indice == 5 && perto(s, 0.894427));
    verificar("MAI-02", "em empate escolhe o menor indice",
              mais_similar(&v[0], v, 7, &indice, &s) == OK && indice == 5);
    verificar("MAI-03", "funciona se todos forem negativos",
              mais_similar(&v[0], v, 4, &indice, &s) == OK && indice == 3 && perto(s, -1));
    verificar("MAI-04", "rejeita conjunto so com invalidos e a consulta",
              mais_similar(&v[0], v, 3, &indice, &s) == ERRO_SEM_CANDIDATO);
    verificar("MAI-05", "rejeita consulta nula",
              mais_similar(&v[1], v, 7, &indice, &s) == ERRO_VETOR_NULO);
    verificar("MAI-06", "rejeita conjunto vazio",
              mais_similar(&v[0], v, 0, &indice, &s) == ERRO_SEM_CANDIDATO);
    verificar("MAI-07", "aceita consulta fora do conjunto",
              mais_similar(&v[0], v + 4, 3, &indice, &s) == OK && indice == 1);
    remover(&v[0], 0, NULL);
    remover(&v[0], 0, NULL);
    verificar("MAI-08", "rejeita consulta vazia",
              mais_similar(&v[0], v, 7, &indice, &s) == ERRO_VAZIO);
    for (i = 0; i < 7; i++) liberar_array(&v[i]);
}

int main(void) {
    testar_inicializar_array();
    testar_inserir();
    testar_imprimir();
    testar_buscar();
    testar_remover();
    testar_multiplicacao();
    testar_soma();
    testar_produto();
    testar_norma();
    testar_similaridade();
    testar_mais_similar();
    printf("\nUnitarios: %d testes, %d falhas\n", total, falhas);
    return falhas != 0;
}
