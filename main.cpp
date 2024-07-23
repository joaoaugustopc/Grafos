#include "source/Graph.cpp"



int main(int argc, char* argv[])
{
    std::ifstream instance("instances_example/5nD.dat");

    if(!instance.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo de instância" << std::endl;
        return 1;
    }
    
    Graph *graph = new Graph(instance);

    std :: ofstream dotFile("graph.dot");

    graph->print_graph(dotFile);

    std::cout <<"Numero de nós do grafo: "<< graph->get_number_of_nodes() << std::endl;
    std::cout <<"Numero de arestas do grafo: "<< graph->get_number_of_edges() << std::endl;
    
    if(graph->conected(1,2) >= 0){
        std::cout << "Os nós 1 e 2 estão conectados com peso " << graph->conected(1,2) << std::endl;
    }
    else{
        std::cout << "Os nós 1 e 2 não estão conectados" << std::endl;
    }

    delete graph;

    return 0;
}
