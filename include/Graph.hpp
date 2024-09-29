#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "Node.hpp"
#include "defines.hpp"

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
    bool                            conected(size_t node_id_1, size_t node_id_2);
    std::vector<size_t>             transitive_closure(size_t node_id);
    std::vector<size_t>             transitive_indirect(size_t node_id);
    std::vector<size_t>             edsger_dijkstra(size_t node_id_1, size_t node_id_2);
    std::vector<size_t>             floyd_warshall(size_t node_id_1, size_t node_id_2);
    Graph                           *prim(size_t start_node_id);
    Graph                           *kruscal(std::vector<size_t> nodes_ids);
    std::vector<std::vector<float>> distancias_minimas();
    std::vector<float>              get_excentricidades();
    float                           get_diametro();
    float                           get_raio();
    std::vector<size_t>             get_centro();
    std::vector<size_t>             get_periferia();
    std::vector<int> findArticulationPoints();
    void DFS_ArticulationPoints(int node_id, std::map<int, bool>& visited, std::map<int, int>& discoveryTime,
                                    std::map<int, int>& lowTime, std::map<int, int>& parent, std::vector<int>& articulationPoints, int& time);
    
    void new_read(std::ifstream& instance);
    bool is_tabu(const std::tuple<int, int, int>& move, const std::map<int, std::map<int, int>>& tabuMatrix,
                int iteration,int l_in);
    std::vector<std::vector<int>> tabu_search(const std::vector<std::vector<int>>& adjList, const std::vector<int>& vertexWeights,
                                          const std::vector<std::vector<int>>& initial_partition, int max_iter, int l_min, int l_max);

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


    void initialize_tabu_matrix(std::map<int, std::map<int, int>>& tabuMatrix, int numVertices, int numSubgraphs);
    bool is_articulation_vertex(int v, const std::vector<int>& subgraph, const std::vector<std::vector<int>>& adjList);
    std::vector<std::tuple<int, int, int>> generate_neighborhood(const std::vector<std::vector<int>>& adjList,
                                                            const std::vector<std::vector<int>>& current_solution);
    void evaluate_moves(const std::vector<std::tuple<int, int, int>>& neighborhood_moves,
    const std::vector<std::vector<int>>& current_solution, const std::map<int, std::map<int, int>>& tabuMatrix,
    int iteration, int l_in, const std::vector<std::vector<int>>& best_solution, std::tuple<int, int,int>& best_move,
    double& best_gap, const std::vector<int>& vertexWeights);
    void apply_move(const std::tuple<int, int, int>& move, std::vector<std::vector<int>>& current_solution);
    double compute_total_gap(const std::vector<std::vector<int>>& solution, const std::vector<int>& vertexWeights);
    void update_tabu_matrix(std::map<int, std::map<int, int>>& tabuMatrix, const std::tuple<int, int, int>& move,
                        int iteration);
};

#endif  //GRAPH_HPP
