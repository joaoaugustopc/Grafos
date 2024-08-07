#include "source/Graph.cpp"

int main(int argc, char *argv[])
{
    
    if (argc < 5)
    {
        std::cerr << "Uso: " << argv[0] << " <nome_da_instancia>" << std::endl;
        return 1;
    }

    std::ifstream input(argv[1]);
    std::ofstream output(argv[2]);
    bool directed = std::stoi(argv[3]);
    bool weighted_edges = std::stoi(argv[4]);
    bool weighted_nodes = std::stoi(argv[5]);

    if (!input.is_open())
    {
        std::cerr << "Erro ao abrir um arquivo" << std::endl;
        return 1;
    }
    
    Graph *graph = new Graph(input, directed, weighted_edges, weighted_nodes);
    
    graph->print_graph(output);
    graph->print_graph();

    std::cout << "Numero de noss do grafo: " << graph->get_number_of_nodes() << std::endl;
    std::cout << "Numero de arestas do grafo: " << graph->get_number_of_edges() << std::endl;

    
    std::ofstream dotFile("output/dot/DFS.dot");
    std::ofstream krscal("output/dot/krscal_6nU.dot");
    
    graph->busca_prof(1, dotFile);
    graph->kruscal({1, 2, 3, 4, 5, 6}, krscal);

    delete graph;

    return 0;
}

//output/png/