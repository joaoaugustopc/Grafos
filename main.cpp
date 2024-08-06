#include "source/Graph.cpp"

int main(int argc, char *argv[])
{
    std::ifstream instance("instances_example/5nD.dat");

    if (!instance.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo de instância" << std::endl;
        return 1;
    }

    Graph *graph = new Graph();

    graph->add_edge(1,2);
    graph->add_edge(2,1);
    graph->add_edge(1,3);
    graph->add_edge(3,1);
    graph->add_edge(2,4);
    graph->add_edge(4,2);
    graph->add_edge(2,5);
    graph->add_edge(5,2);
    graph->add_edge(3,6);
    graph->add_edge(6,3);
    graph->add_edge(3,7);
    graph->add_edge(7,3);
    graph->add_edge(4,8);
    graph->add_edge(8,4);
    graph->add_edge(5,8);
    graph->add_edge(6,8);
    graph->add_edge(7,8);
    graph->add_edge(8,5);
    graph->add_edge(8,6);
    graph->add_edge(8,7);
    

    std ::ofstream dotFile("graph.dot");

    graph->print_graph(dotFile);

    std::cout << "Numero de nós do grafo: " << graph->get_number_of_nodes() << std::endl;
    std::cout << "Numero de arestas do grafo: " << graph->get_number_of_edges() << std::endl;

    if (graph->conected(1, 2) >= 0)
    {
        std::cout << "Os nós 1 e 2 estão conectados com peso " << graph->conected(1, 2) << std::endl;
    }
    else
    {
        std::cout << "Os nós 1 e 2 não estão conectados" << std::endl;
    }

    std::ofstream dotFile2("arvore.dot");
    
    graph->busca_prof(1, dotFile2);

    delete graph;

    return 0;
}
