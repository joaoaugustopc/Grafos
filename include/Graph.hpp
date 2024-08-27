#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "Node.hpp"
#include "defines.hpp"
#include <sstream>

class Graph
{
public:
    /*Assinatura dos métodos básicos para o funcionamento da classe*/

    Graph(std::ifstream& instance, bool directed, bool weighted_edges, bool weighted_nodes);
    Graph(bool directed, bool weighted_edges, bool weighted_nodes);
    ~Graph();

    void                            remove_node(size_t node_id);
    void                            remove_edge(size_t node_id_1, size_t node_id_2);
    void                            add_node(size_t node_id, float weight = 1);
    void                            add_edge(size_t node_id_1, size_t node_id_2, float weight = 1, bool reverse = false);
    void                            print_graph(std::ofstream& output_file);
    void                            print_graph();
    int                             get_number_of_nodes();
    int                             get_number_of_edges();
    void                            busca_prof(size_t node_id, std::ofstream& output_file);
    Graph                          *kruscal(std::vector<size_t> nodes_ids);
    bool                            conected(size_t node_id_1, size_t node_id_2);
    std::vector<size_t>             transitive_closure(size_t node_id);
    std::vector<size_t>             transitive_indirect(size_t node_id);
    std::vector<size_t>             floyd_warshall(size_t node_id_1, size_t node_id_2);
    std::vector<size_t>             edsger_dijkstra(size_t node_id_1, size_t node_id_2);
    std::vector<std::vector<float>> distancias_minimas();
    std::vector<float>              get_excentricidades();
    float                           get_diametro();
    float                           get_raio();
    std::vector<size_t>             get_centro();
    std::vector<size_t>             get_periferia();
    Graph *prim(size_t start_node_id);

private:
    size_t _number_of_nodes;
    size_t _number_of_edges;
    bool   _directed;
    bool   _weighted_edges;
    bool   _weighted_nodes;
    Node  *_first;
    Node  *_last;
    void   DFS(Node *node, size_t node_id, size_t parent, std::map<size_t, bool>& visited, std::ofstream& output_file,
               std::set<std::pair<size_t, size_t>>& printed_return_edges, std::vector<std::tuple<size_t, size_t, float>>& arv,
               std::vector<std::tuple<size_t, size_t, float>>& retorno);
    void   induced_subgraph(std::vector<size_t> nodes_ids, std::vector<std::tuple<int, int, float>>& edges);
    int    search(std::map<int, int>& components, int i);
    void   Union(std::map<int, int>& components, int x, int y);
    void   DFS_TC(size_t node_id, std::map<size_t, bool>& visited, std::vector<size_t>& stack);

    Node                           *create_node(size_t node_id, float weight);
    Node                           *find_node(size_t node_id);
    Edge                           *create_edge(size_t target_id, float weight = 0);
    void                            vectorToDotFile(std::ofstream& output_file, std::vector<std::tuple<size_t, size_t, float>> *arvore = nullptr,
                                                    std::vector<std::tuple<size_t, size_t, float>> *arestas_retorno = nullptr);
    std::vector<std::vector<float>> create_matrix();
    std::vector<std::vector<float>> create_path_matrix();
};

#endif  //GRAPH_HPP
