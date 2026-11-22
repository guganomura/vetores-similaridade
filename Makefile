CC ?= cc
CFLAGS ?= -std=c99 -Wall -Wextra -pedantic
LDLIBS = -lm

ESTRUTURA = src/array.c src/operacoes.c
CABECALHOS = src/array.h src/operacoes.h

all: build/vetores

build/vetores: src/main.c $(ESTRUTURA) $(CABECALHOS)
	@mkdir -p build
	$(CC) $(CFLAGS) src/main.c $(ESTRUTURA) -o $@ $(LDLIBS)

build/testes: tests/test_unitarios.c $(ESTRUTURA) $(CABECALHOS)
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_unitarios.c $(ESTRUTURA) -o $@ $(LDLIBS)

run: build/vetores
	./build/vetores

test: build/testes build/vetores
	./build/testes
	sh tests/test_sistema.sh

clean:
	rm -rf build

.PHONY: all run test clean
