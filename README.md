# Grafos
Este repositório contém a implementação dos algoritmos estudados na disciplina de Grafos. O objetivo é fornecer exemplos práticos e didáticos para o entendimento e aplicação de conceitos teóricos abordados ao longo do curso.

# Instruções para compilar e rodar:

Seguimos o padrão solicitado pela professora da Disciplina DCC059 - Teoria dos Grafos,
compilando com makefile, executando com os arquivos de entrada, saída e pesos. Como exemplo:

Compilação: make
Execução: ./graph_project instances_example/6nU.dat output.txt 0 1 1

0 indica que o grafo é não direcionado.
1 indica que o grafo é ponderado nas arestas.
1 indica que o grafo é ponderado nos vértices.

- É claro que 6nU.dat é uma escolha arbitrária e, você pode (assim como deve) escolher o arquivo de interesse que esteja dentro de "instances_example".

Após compilar e rodar, bem como quando realizar qualquer mudança no grafo original, recomendamos que exiba o grafo (opção 5).

# Bugs:

1- Escreve o grafo no arquivo somente quando mata o programa ou quando seleciona a opcao 5 (Exibir grafo). Por isso, recomendamos que, ao compilar e rodar e aparecer o menu, aperte a opção 5 (exibir grafo) antes de fazer qualquer coisa. Caso haja modificações no grafo (inserção, remoção ou demais mudanças), recomendamos a utilização da opção 5 novamente.

2- 