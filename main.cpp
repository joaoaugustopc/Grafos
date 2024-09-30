#include "include/Graph.hpp"

void clear_screen()
{
    system("cls");
}

void print_line(char edge_char, char fill_char, int width)
{
    std::cout << edge_char << std::string(width, fill_char) << edge_char << std::endl;
}

void print_centered_text(const std::string& text, int width)
{
    int padding = (width - text.length()) / 2;
    std::cout << "|" << std::setw(padding + text.length()) << text << std::setw(width - padding - text.length() + 1) << "|" << std::endl;
}

void saveOutputToFile(Graph *graph)
{
    std::string filename;
    std::cout << "Digite o nome do arquivo para salvar a saida: ";
    std::cin >> filename;
    std::ofstream outputFile(filename);
    if (outputFile.is_open())
    {
        graph->print_graph(outputFile);
        outputFile.close();
        std::cout << "Saida salva em " << filename << std::endl;
    }
    else
    {
        std::cerr << "Erro ao abrir o arquivo " << filename << std::endl;
    }
}

int print_menu()
{
    const int width = 108;  // Largura do menu

    print_line('+', '-', width);
    print_centered_text("MENU", width);
    print_line('+', '-', width);
    print_centered_text("1. Algoritmo Guloso;", width);
    print_centered_text("2. Grasp;", width);
    print_line('+', '-', width);
    print_centered_text("Escolha uma opcao (1-2)", width);
    print_centered_text("opcao (0) para sair", width);
    print_line('+', '-', width);
    std::cout << "> Digite a opcao desejada: ";
    int op;
    std::cin >> op;

    return op;
}

/*
int print_menu()
{
    const int width = 108;  // Largura do menu

    print_line('+', '-', width);
    print_centered_text("MENU", width);
    print_line('+', '-', width);
    print_centered_text("1. Adicionar vertice;", width);
    print_centered_text("2. Adicionar aresta;", width);
    print_centered_text("3. Remover vertice;", width);
    print_centered_text("4. Remover aresta;", width);
    print_centered_text("5. Exibir grafo;", width);
    print_centered_text("6. Fecho transitivo direto de um vertice;", width);
    print_centered_text("7. Fecho transitivo indireto de um vertice;", width);
    print_centered_text("8. Caminho minimo entre dois vertices. (versao: Dijkstra);", width);
    print_centered_text("9. Caminho minimo entre dois vertices. (Versao: Floyd);", width);
    print_centered_text("10. Arvore Geradora Minima. (Versao: Prim);", width);
    print_centered_text("11. Arvore Geradora Minima. (Versao: Kruscal);", width);
    print_centered_text("12. Caminhamento em profundidade;", width);
    print_centered_text("13. Raio, diametro, centro e periferia do grafo; ", width);
    print_centered_text("14. Conjunto de vertices de articulacao; ", width);
    print_line('+', '-', width);
    print_centered_text("Escolha uma opcao (1-14)", width);
    print_centered_text("opcao (0) para sair", width);
    print_line('+', '-', width);
    std::cout << "> Digite a opcao desejada: ";
    int op;
    std::cin >> op;

    return op;
}
*/

void saveGraphToFile(Graph *graph, const std::string& filename)
{
    std::ofstream outputFile(filename);
    if (outputFile.is_open())
    {
        graph->print_graph(outputFile);
        outputFile.close();
        std::cout << "Grafo salvo em " << filename << std::endl;
    }
    else
    {
        std::cerr << "Erro ao abrir o arquivo " << filename << std::endl;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        std::cerr << "Uso: " << argv[0] << " <nome_da_instancia>" << std::endl;
        return 1;
    }

    std::ifstream input(argv[1]);
    std::ofstream output;
    bool          directed       = std::stoi(argv[3]);
    bool          weighted_edges = std::stoi(argv[4]);
    bool          weighted_nodes = std::stoi(argv[5]);
    std::cout << "Carregando grafo..." << std::endl;

    if (!input.is_open())
    {
        std::cerr << "Erro ao abrir um arquivo" << std::endl;
        return 1;
    }

    Graph *graph = new Graph(input, directed, weighted_edges, weighted_nodes);

    std::cout << "Grafo carregado com sucesso" << std::endl;

    time_t start, end;
    double tempoexec;
    std::vector<std::vector<int>> solution;

    int choice;

    do
    {
        choice = print_menu();
        switch (choice)
        {
            case 0:
                break;
            case 1:
                time(&start);
                graph->mggp();
                time(&end);
                tempoexec = difftime(end, start);
                std::cout << std::fixed << std::setprecision(2) << "Tempo de execucao: " << tempoexec << std::endl;

                break;

            case 2:
                time(&start);
                solution = graph->grasp(1000, 0.5);
                time(&end);
                tempoexec = difftime(end, start);
                std::cout << std::fixed << std::setprecision(2) << "Tempo de execucao: " << tempoexec << std::endl;

                break;
            default:
                std::cout << "Opcao invalida" << std::endl;
                break;
        }

        std::cout << "Pressione enter para continuar..." << std::endl;
        std::cout << "> ";
        std::cin.ignore();
        std::cin.get();
    } while (choice != 0);

    /*
    int                 choice;
    std::vector<size_t> nodes;
    std::ofstream       kruscal;
    std::ofstream       DFS;
    size_t              node_id;
    size_t              source_id, target_id;
    float               weight;
    Graph              *subgraph;
    char                c = 'U';
    std::vector<size_t> caminho;
    float               raio, diametro;
    std::vector<size_t> centro, periferia;

    do
    {
        choice = print_menu();
        switch (choice)
        {
            case 0:
                break;
            case 1:
                std::cout << "Digite o id do vertice: ";
                std::cin >> node_id;
                graph->add_node(node_id);

                saveGraphToFile(graph, argv[2]);

                break;
                
            case 2:
                std::cout << "Digite o id do vertice de origem: ";
                std::cin >> source_id;
                std::cout << "Digite o id do vertice de destino: ";
                std::cin >> target_id;
                if (*argv[4] == true)
                {
                    std::cout << "Digite o peso da aresta: ";
                    std::cin >> weight;
                    graph->add_edge(source_id, target_id, weight);
                }
                else
                {
                    graph->add_edge(source_id, target_id);
                }

                saveGraphToFile(graph, argv[2]);

                break;

            case 3:
                std::cout << "Digite o id do vertice: ";

                std::cin >> node_id;

                std ::cout << "Numero de vertices do Grafo: " << graph->get_number_of_nodes() << std::endl;

                std::cout << "Removendo vertice " << node_id << "..." << std::endl;
                graph->remove_node(node_id);

                std ::cout << "Numero de vertices do Grafo: " << graph->get_number_of_nodes() << std::endl;

                saveGraphToFile(graph, argv[2]);

                break;

            case 4:
                std::cout << "Digite o id do vertice de origem: ";
                std::cin >> source_id;
                std::cout << "Digite o id do vertice de destino: ";
                std::cin >> target_id;
                std ::cout << "Numero de arestas do Grafo: " << graph->get_number_of_edges() << std::endl;
                graph->remove_edge(source_id, target_id);
                std::cout << "Removendo aresta " << source_id << " -- " << target_id << "..." << std::endl;
                std ::cout << "Numero de arestas do Grafo: " << graph->get_number_of_edges() << std::endl;

                saveGraphToFile(graph, argv[2]);

                break;

            case 5:
                saveGraphToFile(graph, argv[2]);
                break;

            case 6:
                std::cout << "Digite o id do vertice: ";
                std::cin >> node_id;
                std::cout << "Fecho transitivo direto do vertice " << node_id << ": ";
                for (auto i : graph->transitive_closure(node_id))
                {
                    std::cout << i << ", ";
                }
                std::cout << std::endl;

                saveGraphToFile(graph, argv[2]);

                break;

            case 7:
                std::cout << "Digite o id do vertice: ";
                std::cin >> node_id;
                std::cout << "Fecho transitivo indireto do vertice " << node_id << ": ";
                for (auto i : graph->transitive_indirect(node_id))
                {
                    std::cout << i << ", ";
                }
                std::cout << std::endl;

                saveGraphToFile(graph, argv[2]);

                break;

            case 8:
                std::cout << "Digite o id do vertice de origem: ";
                std::cin >> source_id;
                std::cout << "Digite o id do vertice de destino: ";
                std::cin >> target_id;
                caminho = graph->edsger_dijkstra(source_id, target_id);
                if (caminho.empty())
                {
                    std::cout << "Nao ha caminho entre " << source_id << " e " << target_id << "." << std::endl;
                } else {
                    std::cout << "Caminho minimo entre " << source_id << " e " << target_id << ": ";
                    for (size_t i = 0; i < caminho.size(); ++i) {
                        std::cout << caminho[i];
                        if (i < caminho.size() - 1) {
                            std::cout << " -> ";
                        }
                    }
                    std::cout << std::endl;
                }

                saveGraphToFile(graph, argv[2]);

                break;

            case 9:
                std::cout << "Digite o id do vertice de origem: ";
                std::cin >> source_id;
                std::cout << "Digite o id do vertice de destino: ";
                std::cin >> target_id;
                caminho = graph->floyd_warshall(source_id, target_id);
                std::cout << "Caminho minimo entre " << source_id << " e " << target_id << ": ";
                for (auto i : caminho)
                {
                    std::cout << i << ", ";
                }
                std::cout << std::endl;
                break;

            case 10:
                std::cout << "Digite o id do vertice de origem para o algoritmo de Prim: ";
                std::cin >> source_id;

                subgraph = graph->prim(source_id);

                if (subgraph->get_number_of_edges() == 0) {
                    std::cout << "Nao foi possivel gerar a arvore geradora minima a partir do vertice " << source_id << "." << std::endl;
                } else {
                    std::cout << "Arvore Geradora Minima (Versao: Prim):" << std::endl;
                    subgraph->print_graph();
                }
                delete subgraph;

                saveGraphToFile(graph, argv[2]);

                break;

            case 11:
                if (*argv[3] != '0')
                {
                    c = 'D';
                }
                kruscal.open("output/dot/kruscal_" + std::to_string(graph->get_number_of_nodes()) + "n" + c + ".dot");
                if (kruscal.is_open())
                {
                    std::cout << "Arvore Geradora Minima (Versao: Kruscal): ";
                    std::cout << "tamanho do conjunto X de vertices: ";
                    int tam;
                    std::cin >> tam;
                    for (int i = 0; i < tam; i++)
                    {
                        std::cout << "Digite o id do vertice: ";
                        std::cin >> node_id;
                        nodes.push_back(node_id);
                    }
                    subgraph = graph->kruscal(nodes);
                    subgraph->print_graph(kruscal);

                    delete subgraph;
                }
                else
                {
                    std::cerr << "Erro ao abrir o arquivo de saida" << std::endl;
                }
                kruscal.close();

                saveGraphToFile(graph, argv[2]);

                break;
            case 12:
                if (*argv[3] != '0')
                {
                    c = 'D';
                }
                DFS.open("output/dot/DFS_" + std::to_string(graph->get_number_of_nodes()) + "n" + c + ".dot");
                if (DFS.is_open())
                {
                    std::cout << "Digite o id do vertice: ";
                    std::cin >> node_id;
                    graph->busca_prof(node_id, DFS);
                }
                else
                {
                    std::cerr << "Erro ao abrir o arquivo de saida" << std::endl;
                }

                saveGraphToFile(graph, argv[2]);
                
                break;
            case 13:
                raio      = graph->get_raio();
                diametro  = graph->get_diametro();
                centro    = graph->get_centro();
                periferia = graph->get_periferia();

                std::cout << "Raio do grafo: " << raio << std::endl;
                std::cout << "Diametro do grafo: " << diametro << std::endl;
                std::cout << "Centro do grafo: ";
                for (auto i : centro)
                {
                    std::cout << i << ", ";
                }
                std::cout << std::endl;
                std::cout << "Periferia do grafo: ";
                for (auto i : periferia)
                {
                    std::cout << i << ", ";
                }
                std::cout << std::endl;
                
                saveGraphToFile(graph, argv[2]);

                break;
            case 14:
                std::cout << "Conjunto de vertices de articulacao: ";
                for (auto i : graph->findArticulationPoints())
                {
                    std::cout << i << ", ";
                }

                saveGraphToFile(graph, argv[2]);

                break;
            default:
                std::cout << "Opcao invalida" << std::endl;
                break;
        }

        std::cout << "Pressione enter para continuar..." << std::endl;
        std::cout << "> ";
        std::cin.ignore();
        std::cin.get();
    } while (choice != 0);

    delete graph;

    output.close();
    */
    /*
    */
    return 0;
}