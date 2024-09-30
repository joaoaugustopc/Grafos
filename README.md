# Grafos
Este repositório contém a implementação dos algoritmos estudados na disciplina de Grafos e Heurísticas, focando na resolução do problema de Particionamento de Grafos com Mínima Diferença (Minimum Gap Graph Partitioning Problem - MGGPP).

O objetivo deste projeto é fornecer exemplos práticos e didáticos que auxiliem no entendimento e na aplicação dos conceitos teóricos abordados ao longo do curso. Aqui, você encontrará implementações que ilustram as estratégias utilizadas, facilitando a aprendizagem e a aplicação dos algoritmos.

# Instruções para compilar e rodar:

Seguimos o padrão solicitado pela professora da Disciplina DCC059 - Teoria dos Grafos,
compilando com makefile, executando com os arquivos de entrada, saída e pesos. Como exemplo:

Compilação: make
Execução: ./graph_project Instaces_MGGP/n100d03p1i1.txt output.txt 0 1 1

0 indica que o grafo é não direcionado.
1 indica que o grafo é ponderado nas arestas.
1 indica que o grafo é ponderado nos vértices.

- É claro que n100d03p1i1.txt é uma escolha arbitrária e, você pode (assim como deve) escolher o arquivo de interesse que esteja dentro de "Instaces_MGGP".
