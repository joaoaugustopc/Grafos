#ifndef GRAFO_BASICO_EDGE_H
#define GRAFO_BASICO_EDGE_H

#include "defines.hpp"

struct Edge
{
    Edge  *_next_edge;
    float  _weight;
    size_t _target_id; // Grafo sempre é direcionado ?
    
    //não é um ponteiro, isso nao afetaria o desenpenho ? Pois para fazer o caminhamento no grafo
    //eu teria que percorrer todos os nós para encontrar o nó que eu quero, e isso é muito custoso.  
    // Sugestão: Utilizar uma tabela hash para armazenar os nós, assim a busca seria O(1) e não O(n) #include <unordered_map>

};

#endif /* GRAFO_BASICO_EDGE_H */