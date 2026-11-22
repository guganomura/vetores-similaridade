# Vetores e similaridade de cosseno

Aplicação em C que armazena vetores de números reais (`float`) em arrays, realiza operações
matemáticas sobre eles e calcula a similaridade de cosseno. Usa apenas a biblioteca padrão do C.

- [Compilação e execução](#compilação-e-execução)
- [A estrutura de dados](#a-estrutura-de-dados)
- [Funcionalidades](#funcionalidades)
- [Exemplos de execução](#exemplos-de-execução)
- [Testes realizados](#testes-realizados)

## Compilação e execução

É necessário um compilador C99 (GCC ou Clang).

Com Make (macOS e Linux):

```bash
make        # compila
make run    # executa
make test   # roda todos os testes
make clean  # apaga os binários
```

Sem Make:

```bash
mkdir -p build
cc -std=c99 src/main.c src/array.c src/operacoes.c -o build/vetores -lm
./build/vetores

cc -std=c99 tests/test_unitarios.c src/array.c src/operacoes.c -o build/testes -lm
./build/testes
sh tests/test_sistema.sh
```

No Windows, com MinGW:

```bat
gcc -std=c99 src/main.c src/array.c src/operacoes.c -o vetores.exe -lm
vetores.exe
```

## Organização

```text
src/array.h, array.c            estrutura de dados
src/operacoes.h, operacoes.c    operações matemáticas
src/main.c                      menu, leitura e validação da entrada
tests/test_unitarios.c          testes da estrutura e das operações
tests/test_sistema.sh           testes do programa completo
```

## A estrutura de dados

```c
typedef struct {
    float *dados;
    int tamanho;
    int capacidade;
} Array;
```

`dados` aponta para um bloco contíguo de `float`. Como todos os elementos têm o mesmo tamanho,
o endereço de `dados[i]` é `início + i * sizeof(float)`, o que dá acesso direto em O(1).

```text
capacidade = 5, tamanho = 3

índice    0     1     2     3     4
       +-----+-----+-----+-----+-----+
dados  | 0.8 | 0.2 | 0.5 |     |     |
       +-----+-----+-----+-----+-----+
```

| Operação | O que faz | Tempo |
|---|---|---|
| `inicializar_array` | aloca a capacidade pedida, com tamanho 0 | O(1), sem contar a alocação |
| `inserir` | desloca à direita e grava no índice | O(n); O(1) no fim |
| `imprimir` | percorre e escreve os elementos | O(n) |
| `buscar` | busca linear pela primeira ocorrência | O(n) |
| `remover` | retira do índice e desloca à esquerda | O(n); O(1) no fim |

`liberar_array` é uma função auxiliar, necessária em C para devolver a memória.

## Funcionalidades

| Requisito | Opção do menu | Função principal | Tempo |
|---|:-:|---|---|
| 1. Informar o tamanho | 1 | `inicializar_array` | O(1) |
| 2. Criar dois ou mais vetores | 1 | `criar_vetores` | O(m·n) |
| 3. Informar os valores | 1 | `inserir` | O(n) |
| 4. Exibir os vetores | 2 | `imprimir` | O(m·n) |
| 5. Multiplicar por escalar | 3 | `multiplicar_escalar` | O(n) |
| 6. Somar | 4 | `somar` | O(n) |
| 7. Produto escalar | 5 | `produto_escalar` | O(n) |
| 8. Norma | 6 | `norma` | O(n) |
| 9. Similaridade de cosseno | 7 | `similaridade_cosseno` | O(n) |
| 10. Vetor mais similar | 8 | `mais_similar` | O(m·n) |

`n` é a dimensão e `m` é a quantidade de vetores. As opções 9, 10 e 11 do menu dão acesso direto a
`buscar`, `inserir` e `remover`.

### Fórmulas

```text
multiplicação   B[i] = k * A[i]
soma            C[i] = A[i] + B[i]
produto         A . B = A[0]*B[0] + ... + A[n-1]*B[n-1]
norma           ||A|| = raiz(A[0]^2 + ... + A[n-1]^2)
cosseno         cos(A, B) = (A . B) / (||A|| * ||B||)
```

### Decisões de projeto

- **Capacidade fixa.** Um array cheio rejeita inserções. Remover um elemento abre espaço e
  reduz a dimensão do vetor.
- **Dimensões diferentes** são rejeitadas na soma, no produto escalar e no cosseno.
- **Vetor nulo** não tem similaridade de cosseno, pois haveria divisão por zero.
- **Vetor mais similar.** A consulta é um dos vetores armazenados e não é comparada consigo
  mesma. Vetores nulos, vazios ou de outra dimensão são ignorados. Empates ficam com o menor índice.
- **Precisão.** Os elementos são `float`, e os acumuladores são `double` para evitar estouro.
- **Busca** usa tolerância relativa, por causa do arredondamento de `float`.
- **Entrada.** Aceita ponto ou vírgula decimal e recusa texto, infinito e NaN.
- **Limites.** Até 100 vetores, cada um com até 10000 posições.

### Observação sobre o exemplo do enunciado

O enunciado indica `0.9876` para A = `[0.8, 0.2, 0.5, 0.9]` e B = `[0.7, 0.1, 0.6, 0.8]`.
Pela fórmula da similaridade de cosseno, o valor é `0.9904`:

```text
A . B    = 0.56 + 0.02 + 0.30 + 0.72 = 1.60
||A||^2  = 0.64 + 0.04 + 0.25 + 0.81 = 1.74
||B||^2  = 0.49 + 0.01 + 0.36 + 0.64 = 1.50
cos      = 1.60 / raiz(1.74 * 1.50)  = 0.990375...
```

O programa e os testes usam o valor calculado.

## Exemplos de execução

O menu reaparece após cada operação e foi omitido dos exemplos, exceto na primeira vez.

### Exemplo 1: similaridade, vetor nulo e escalar

```text
=== VETORES E SIMILARIDADE DE COSSENO ===
 1. Criar vetores
 2. Exibir vetores
 3. Multiplicar por escalar
 4. Somar dois vetores
 5. Produto escalar
 6. Norma
 7. Similaridade de cosseno
 8. Encontrar o mais similar
 9. Buscar valor em um vetor
10. Inserir valor em um vetor
11. Remover valor de um vetor
 0. Sair
Opcao: 1
Quantos vetores deseja criar? 3
Tamanho de V1: 4
  V1[0] = 0.8
  V1[1] = 0.2
  V1[2] = 0.5
  V1[3] = 0.9
V1 criado (dimensao 4).
Tamanho de V2: 4
  V2[0] = 0.7
  V2[1] = 0.1
  V2[2] = 0.6
  V2[3] = 0.8
V2 criado (dimensao 4).
Tamanho de V3: 4
  V3[0] = 0
  V3[1] = 0
  V3[2] = 0
  V3[3] = 0
V3 criado (dimensao 4).

Opcao: 2
V1 (dimensao 4): [0.8000, 0.2000, 0.5000, 0.9000]
V2 (dimensao 4): [0.7000, 0.1000, 0.6000, 0.8000]
V3 (dimensao 4): [0.0000, 0.0000, 0.0000, 0.0000]

Opcao: 7
Primeiro vetor: 1
Segundo vetor: 2
Similaridade: 0.9904

Opcao: 7
Primeiro vetor: 1
Segundo vetor: 3
Operacao invalida: vetor nulo nao possui similaridade de cosseno.

Opcao: 8
Vetor de consulta: 1
Mais similar a V1: V2 (similaridade 0.9904)

Opcao: 3
Numero do vetor: 1
Escalar k: 2
Resultado: [1.6000, 0.4000, 1.0000, 1.8000]
Salvar como novo vetor? (s/n): s
Resultado salvo como V4.

Opcao: 0
Programa encerrado.
```

### Exemplo 2: soma, produto, norma e operações do array

```text
Opcao: 1
Quantos vetores deseja criar? 3
Tamanho de V1: 3
  V1[0] = 1
  V1[1] = 2
  V1[2] = 3
V1 criado (dimensao 3).
Tamanho de V2: 3
  V2[0] = 4
  V2[1] = 5
  V2[2] = 6
V2 criado (dimensao 3).
Tamanho de V3: 2
  V3[0] = 3
  V3[1] = 4
V3 criado (dimensao 2).

Opcao: 4
Primeiro vetor: 1
Segundo vetor: 2
Resultado: [5.0000, 7.0000, 9.0000]
Salvar como novo vetor? (s/n): n

Opcao: 4
Primeiro vetor: 1
Segundo vetor: 3
Operacao invalida: os vetores possuem dimensoes diferentes.

Opcao: 5
Primeiro vetor: 1
Segundo vetor: 2
Produto escalar: 32.0000

Opcao: 6
Numero do vetor: 3
Norma: 5.0000

Opcao: 9
Numero do vetor: 2
Valor procurado: 5
Valor encontrado no indice 1.

Opcao: 11
Numero do vetor: 2
Indice de remocao: 1
Valor 5.0000 removido.
V2 (dimensao 2): [4.0000, 6.0000]

Opcao: 10
Numero do vetor: 2
Indice de insercao: 0
Valor: 9
V2 (dimensao 3): [9.0000, 4.0000, 6.0000]

Opcao: 0
Programa encerrado.
```

## Testes realizados

São **111 testes automatizados**: 73 unitários e 38 de sistema. Todos passam. Para repetir:

```bash
make test
```

Cada teste imprime `[OK]` ou `[FALHA]` com o identificador das tabelas abaixo, e o comando termina
com erro se algum falhar.

```text
Unitarios: 73 testes, 0 falhas
Sistema: 38 testes, 0 falhas
```

### Estrutura de dados

#### `inicializar_array`

| ID | Entrada | Resultado esperado |
|---|---|---|
| INI-01 | capacidade 3 | array alocado, tamanho 0 |
| INI-02 | capacidade 0 | rejeita |
| INI-03 | capacidade -1 | rejeita |
| INI-04 | ponteiro `NULL` | rejeita |
| INI-05 | liberar duas vezes | array zerado, sem falha |

#### `inserir`

| ID | Entrada | Resultado esperado |
|---|---|---|
| INS-01 | 2 no índice 0 de `[]` | `[2]` |
| INS-02 | 4 no fim de `[2]` | `[2, 4]` |
| INS-03 | 1 no início de `[2, 4]` | `[1, 2, 4]` |
| INS-04 | índice -1 | rejeita |
| INS-05 | índice 4 com tamanho 3 | rejeita |
| INS-06 | 3 no índice 2 de `[1, 2, 4]` | `[1, 2, 3, 4]` |
| INS-07 | array cheio | rejeita sem alterar |
| INS-08 | array não alocado | rejeita |

#### `imprimir`

| ID | Entrada | Resultado esperado |
|---|---|---|
| IMP-01 | `[1, -2.5, 0]` | `[1.0000, -2.5000, 0.0000]` |
| IMP-02 | `[7]` | `[7.0000]` |
| IMP-03 | array vazio | `[]` |

#### `buscar`

| ID | Entrada | Resultado esperado |
|---|---|---|
| BUS-01 | 5 em `[5, -2, 5, 0.3, 9]` | 0, a primeira ocorrência |
| BUS-02 | -2 no mesmo array | 1 |
| BUS-03 | 9 no mesmo array | 4 |
| BUS-04 | 8 no mesmo array | -1 |
| BUS-05 | `float` vizinho de 0.3 | 3 |
| BUS-06 | 1e-8 em `[0]` | -1 |
| BUS-07 | 0 em array vazio | -1 |

#### `remover`

| ID | Entrada | Resultado esperado |
|---|---|---|
| REM-01 | índice 0 de `[1, 2, 3, 4]` | devolve 1, fica `[2, 3, 4]` |
| REM-02 | índice 1 de `[2, 3, 4]` | `[2, 4]` |
| REM-03 | índice -1 | rejeita |
| REM-04 | índice igual ao tamanho | rejeita |
| REM-05 | último de `[2, 4]` | devolve 4, fica `[2]` |
| REM-06 | destino `NULL` | remove normalmente |
| REM-07 | array vazio | rejeita |
| REM-08 | inserir após esvaziar | aceita |

### Operações matemáticas

#### Multiplicação por escalar, com A = `[1, -2, 3]`

| ID | Entrada | Resultado esperado |
|---|---|---|
| MUL-01 | k = 2 | `[2, -4, 6]` |
| MUL-02 | k = 0 | `[0, 0, 0]` |
| MUL-03 | k = -1 | `[-1, 2, -3]` |
| MUL-04 | k = 0.5 | `[0.5, -1, 1.5]` |
| MUL-05 | A após as operações | inalterado |
| MUL-06 | vetor vazio | rejeita |
| MUL-07 | maior `float` vezes 2 | rejeita por estouro |

#### Soma, com A = `[1, 2, 3]` e B = `[4, -5, 6.5]`

| ID | Entrada | Resultado esperado |
|---|---|---|
| SOM-01 | A + B | `[5, -3, 9.5]` |
| SOM-02 | A e B após a soma | inalterados |
| SOM-03 | B + A | mesmo resultado |
| SOM-04 | dimensões 3 e 2 | rejeita |
| SOM-05 | dimensões 2 e 3 | rejeita |
| SOM-06 | vetores vazios | rejeita |
| SOM-07 | maior `float` somado a si mesmo | rejeita por estouro |

#### Produto escalar

| ID | Entrada | Resultado esperado |
|---|---|---|
| PRO-01 | `[1, 2, 3]` e `[4, 5, 6]` | 32 |
| PRO-02 | `[1, 0, 0]` e `[0, -1, 0]` | 0 |
| PRO-03 | `[1, 2, 3]` e `[0, -1, 0]` | -2 |
| PRO-04 | dimensões 3 e 2 | rejeita |
| PRO-05 | vetor vazio | rejeita |

#### Norma

| ID | Entrada | Resultado esperado |
|---|---|---|
| NOR-01 | `[3, 4]` | 5 |
| NOR-02 | `[-3, -4]` | 5 |
| NOR-03 | `[0, 0]` | 0 |
| NOR-04 | maior `float` | o próprio valor, sem estouro |
| NOR-05 | vetor vazio | rejeita |

#### Similaridade de cosseno

| ID | Entrada | Resultado esperado |
|---|---|---|
| COS-01 | vetores do enunciado | 0.990375 |
| COS-02 | mesmos vetores, ordem trocada | 0.990375 |
| COS-03 | vetor com ele mesmo | 1, sem ultrapassar |
| COS-04 | `[1, 0]` e `[0, 1]` | 0 |
| COS-05 | `[1, 0]` e `[-3, 0]` | -1 |
| COS-06 | `[1e-30, 0]` e `[1, 0]` | 1 |
| COS-07 | primeiro vetor nulo | rejeita |
| COS-08 | segundo vetor nulo | rejeita |
| COS-09 | dimensões diferentes | rejeita |
| COS-10 | vetor vazio | rejeita |

#### Vetor mais similar, com consulta Q = `[1, 0]`

| ID | Conjunto | Resultado esperado |
|---|---|---|
| MAI-01 | Q, nulo, outra dimensão, oposto, ortogonal, `[2, 1]` | `[2, 1]`, 0.894427 |
| MAI-02 | dois candidatos empatados | menor índice |
| MAI-03 | único candidato com cosseno -1 | escolhe esse candidato |
| MAI-04 | apenas Q e vetores inválidos | rejeita |
| MAI-05 | consulta nula | rejeita |
| MAI-06 | conjunto vazio | rejeita |
| MAI-07 | consulta fora do conjunto | escolhe `[2, 1]` |
| MAI-08 | consulta vazia | rejeita |

### Sistema

Estes testes enviam entradas ao executável, como um usuário faria, e conferem a saída.

| ID | Requisito | Cenário | Resultado esperado |
|---|---|---|---|
| SIS-01 | 1 | tamanho 3 | V1 com dimensão 3 |
| SIS-02 | 2 | criar dois vetores | V1 e V2 criados |
| SIS-03 | 2 | criar três vetores | V3 criado |
| SIS-04 | 3 | valores -1.25 e 3.5 | `[-1.2500, 3.5000]` |
| SIS-05 | 4 | exibir | vetores com nome e dimensão |
| SIS-06 | 5 | `[1, 2, 3]` vezes 2 | `[2, 4, 6]` |
| SIS-07 | 5 | salvar a multiplicação | aparece como V3 |
| SIS-08 | 6 | `[1, 2, 3] + [4, 5, 6]` | `[5, 7, 9]` |
| SIS-09 | 6 | salvar a soma | aparece como V3 |
| SIS-10 | 6 | dimensões diferentes | rejeita |
| SIS-11 | 7 | exemplo do enunciado | 32.0000 |
| SIS-12 | 7 | dimensões diferentes | rejeita |
| SIS-13 | 8 | `[3, 4]` | 5.0000 |
| SIS-14 | 9 | vetores do enunciado | 0.9904 |
| SIS-15 | 9 | vetor nulo | operação inválida |
| SIS-16 | 9 | dimensões diferentes | rejeita |
| SIS-17 | 10 | três vetores | V3 é o mais similar a V1 |
| SIS-18 | 10 | sem candidato compatível | informa o usuário |
| SIS-19 | 10 | consulta nula | operação inválida |
| SIS-20 | `buscar` | valor existente | índice 1 |
| SIS-21 | `buscar` | valor ausente | não encontrado |
| SIS-22 | `remover` | índice 1 de `[1, 2, 3]` | `[1, 3]` |
| SIS-23 | `inserir` | 9 no início, após remover | `[9, 1, 3]` |
| SIS-24 | `inserir` | array cheio | rejeita |
| SIS-25 | `remover` | array vazio | rejeita |
| SIS-26 | 8 | norma de vetor vazio | rejeita |
| SIS-27 | `remover` | índice inexistente | pede de novo |
| SIS-28 | 4 | exibir sem vetores | avisa |
| SIS-29 | menu | operar sem vetores | avisa |
| SIS-30 | menu | opção 99 | pede de novo |
| SIS-31 | 1 | tamanho 0 | pede de novo |
| SIS-32 | 3 | texto como valor | pede de novo |
| SIS-33 | 3 | infinito | pede de novo |
| SIS-34 | 3 | NaN | pede de novo |
| SIS-35 | 3 | `0,75` | aceita como 0.75 |
| SIS-36 | menu | vetor inexistente | pede de novo |
| SIS-37 | menu | resposta diferente de s/n | pede de novo |
| SIS-38 | menu | fim inesperado da entrada | encerra sem travar |
