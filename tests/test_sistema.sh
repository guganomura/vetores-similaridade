#!/bin/sh
# Executa o programa com entradas prontas e confere a saida de cada cenario.
# Uso: sh tests/test_sistema.sh [executavel]

programa=${1:-build/vetores}
total=0
falhas=0

# testar ID DESCRICAO ENTRADA TRECHO_ESPERADO
testar() {
    total=$((total + 1))
    if saida=$(printf "$3" | "$programa" 2>&1) &&
        printf '%s\n' "$saida" | grep -Fq -- "$4"; then
        echo "[OK] $1 $2"
    else
        falhas=$((falhas + 1))
        echo "[FALHA] $1 $2 (esperado: $4)"
    fi
}

# Entradas reutilizadas: criam os vetores V1 e V2.
AB='1\n2\n3\n1\n2\n3\n3\n4\n5\n6\n'
COS='1\n2\n4\n0.8\n0.2\n0.5\n0.9\n4\n0.7\n0.1\n0.6\n0.8\n'
DIF='1\n2\n2\n1\n2\n3\n1\n2\n3\n'
NULO='1\n2\n2\n0\n0\n2\n1\n2\n'

testar SIS-01 "informa o tamanho" '1\n1\n3\n1\n2\n3\n0\n' "V1 criado (dimensao 3)"
testar SIS-02 "cria dois vetores" "${AB}0\n" "V2 criado (dimensao 3)"
testar SIS-03 "cria mais de dois vetores" '1\n3\n1\n1\n1\n2\n1\n3\n0\n' "V3 criado"
testar SIS-04 "le valores reais" '1\n1\n2\n-1.25\n3.5\n2\n0\n' "[-1.2500, 3.5000]"
testar SIS-05 "exibe os vetores" "${AB}2\n0\n" "V2 (dimensao 3): [4.0000, 5.0000, 6.0000]"
testar SIS-06 "multiplica por escalar" "${AB}3\n1\n2\nn\n0\n" "[2.0000, 4.0000, 6.0000]"
testar SIS-07 "salva a multiplicacao" "${AB}3\n1\n2\ns\n2\n0\n" "V3 (dimensao 3): [2.0000, 4.0000, 6.0000]"
testar SIS-08 "soma" "${AB}4\n1\n2\nn\n0\n" "[5.0000, 7.0000, 9.0000]"
testar SIS-09 "salva a soma" "${AB}4\n1\n2\ns\n2\n0\n" "V3 (dimensao 3): [5.0000, 7.0000, 9.0000]"
testar SIS-10 "rejeita soma incompativel" "${DIF}4\n1\n2\n0\n" "dimensoes diferentes"
testar SIS-11 "produto escalar" "${AB}5\n1\n2\n0\n" "Produto escalar: 32.0000"
testar SIS-12 "rejeita produto incompativel" "${DIF}5\n1\n2\n0\n" "dimensoes diferentes"
testar SIS-13 "norma" '1\n1\n2\n3\n4\n6\n1\n0\n' "Norma: 5.0000"
testar SIS-14 "cosseno com 4 casas" "${COS}7\n1\n2\n0\n" "Similaridade: 0.9904"
testar SIS-15 "rejeita vetor nulo" "${NULO}7\n1\n2\n0\n" "Operacao invalida: vetor nulo"
testar SIS-16 "rejeita cosseno incompativel" "${DIF}7\n1\n2\n0\n" "dimensoes diferentes"
testar SIS-17 "encontra o mais similar" '1\n3\n2\n1\n0\n2\n0\n1\n2\n2\n1\n8\n1\n0\n' "Mais similar a V1: V3"
testar SIS-18 "mais similar sem candidato" "${DIF}8\n1\n0\n" "nenhum outro vetor"
testar SIS-19 "mais similar com consulta nula" "${NULO}8\n1\n0\n" "Operacao invalida: vetor nulo"
testar SIS-20 "busca valor existente" "${AB}9\n1\n2\n0\n" "encontrado no indice 1"
testar SIS-21 "busca valor ausente" "${AB}9\n1\n8\n0\n" "Valor nao encontrado"
testar SIS-22 "remove elemento" "${AB}11\n1\n1\n0\n" "[1.0000, 3.0000]"
testar SIS-23 "insere apos remover" "${AB}11\n1\n1\n10\n1\n0\n9\n0\n" "[9.0000, 1.0000, 3.0000]"
testar SIS-24 "rejeita insercao em array cheio" "${AB}10\n1\n0\n" "array cheio"
testar SIS-25 "rejeita remocao de array vazio" '1\n1\n1\n5\n11\n1\n0\n11\n1\n0\n' "array vazio"
testar SIS-26 "rejeita operacao com vetor vazio" '1\n1\n1\n5\n11\n1\n0\n6\n1\n0\n' "vetor vazio"
testar SIS-27 "repete indice invalido" "${AB}11\n1\n3\n2\n0\n" "entre 0 e 2"
testar SIS-28 "exibir sem vetores" '2\n0\n' "Nenhum vetor armazenado"
testar SIS-29 "operar sem vetores" '6\n0\n' "Nenhum vetor armazenado"
testar SIS-30 "repete opcao invalida" '99\n0\n' "entre 0 e 11"
testar SIS-31 "repete tamanho invalido" '1\n1\n0\n1\n5\n0\n' "entre 1 e 10000"
testar SIS-32 "repete numero invalido" '1\n1\n1\nabc\n5\n0\n' "numero real valido"
testar SIS-33 "rejeita infinito" '1\n1\n1\ninf\n5\n0\n' "numero real valido"
testar SIS-34 "rejeita NaN" '1\n1\n1\nnan\n5\n0\n' "numero real valido"
testar SIS-35 "aceita virgula decimal" '1\n1\n1\n0,75\n2\n0\n' "[0.7500]"
testar SIS-36 "repete vetor inexistente" "${AB}6\n3\n1\n0\n" "entre 1 e 2"
testar SIS-37 "repete resposta diferente de s/n" "${AB}4\n1\n2\nx\nn\n0\n" "Responda com s ou n"
testar SIS-38 "encerra no fim da entrada" '1\n1\n3\n1\n' "Programa encerrado"

echo
echo "Sistema: $total testes, $falhas falhas"
[ "$falhas" -eq 0 ]
