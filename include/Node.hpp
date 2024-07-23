#ifndef GRAFO_BASICO_NODE_H
#define GRAFO_BASICO_NODE_H

#include "Edge.hpp"
#include "defines.hpp"

struct Node
{
    size_t _number_of_edges;
    size_t _id;
    float  _weight;
    Edge  *_first_edge;
    Node  *_next_node;
    Node  *_previous_node;
};

#endif  //GRAFO_BASICO_NODE_H
