#include "source/Graph.cpp"

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
    print_centered_text("1. Adicionar vertice;", width);
    print_centered_text("2. Adicionar aresta;", width);
    print_centered_text("3. Remover vertice;", width);
    print_centered_text("4. Remover aresta;", width);
    print_centered_text("5. Exibir grafo;", width);
    print_centered_text("6. Fecho transitivo direto de um vertice;", width);
    print_centered_text("7. Fecho transitovo indireto de um vertice;", width);
    print_centered_text("8. Caminho minimo entre dois vertices. (versao: Djkstra);", width);
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

int main(int argc, char *argv[])
{
    print_menu();
    if (argc < 5)
    {
        std::cerr << "Uso: " << argv[0] << " <nome_da_instancia>" << std::endl;
        return 1;
    }

    std::ifstream input(argv[1]);
    std::ofstream output(argv[2]);
    bool          directed       = std::stoi(argv[3]);
    bool          weighted_edges = std::stoi(argv[4]);
    bool          weighted_nodes = std::stoi(argv[5]);

    if (!input.is_open() || !output.is_open())
    {
        std::cerr << "Erro ao abrir um arquivo" << std::endl;
        return 1;
    }

    Graph *graph = new Graph(input, directed, weighted_edges, weighted_nodes);

    int                 choice = 0;
    std::vector<size_t> nodes;
    std::ofstream       kruscal("output_kruscal.dot");
    std::ofstream       DFS("output_dfs.dot");
    size_t              node_id;
    size_t              source_id, target_id;
    float               weight;
    Graph              *subgraph;

    //problema: apenas escreve o grafo no arquivo quando mata o programa ou quando ica repetindo a opcao 5

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

                std::cout << "Digite o peso da aresta: ";
                std::cin >> weight;
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
            {
                if (output.is_open())
                {
                    graph->print_graph(output);
                    output << std::endl;
                    std::cout << "Grafo impresso no arquivo de saida" << std::endl;
                    output.close();  // Fecha o arquivo após a escrita
                }
                else
                {
                    std::cerr << "Erro ao abrir o arquivo de saida" << std::endl;
                }
            }
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
                break;
            case 7:
                break;
            case 8:
                break;
            case 9:
                break;
            case 10:
                break;
            case 11:
                std::cout << "Arvore Geradora Minima (Versao: Kruscal): ";
                std::cout << "tamanho do conjunto X de vertices: ";
                int tam;
                std::cin >> tam;
                for (size_t i = 0; i < tam; i++)
                {
                    std::cout << "Digite o id do vertice: ";
                    std::cin >> node_id;
                    nodes.push_back(node_id);
                }
                subgraph = graph->kruscal(nodes);
                subgraph->print_graph(kruscal);

                delete subgraph;
                break;
            case 12:
                graph->busca_prof(1, DFS);
                break;
            case 13:
                break;
            case 14:
                break;
            default:
                std::cout << "Opcao invalida" << std::endl;
                break;
        }
    } while (choice != 0);

    delete graph;

    output.close();
    return 0;
}

/*
    graph->print_graph(output);
    graph->print_graph();

    std::cout << "Numero de noss do grafo: " << graph->get_number_of_nodes() << std::endl;
    std::cout << "Numero de arestas do grafo: " << graph->get_number_of_edges() << std::endl;

    //std::ofstream dotFile("output/dot/DFS.dot");
    //std::ofstream krscal("output/dot/krscal_6nU.dot");

    //graph->busca_prof(1, dotFile);
    //Graph *ArvoreMin = graph->kruscal({ 1, 2, 3, 4, 5, 6 }, krscal);

    std::vector<size_t> vetor = graph->transitive_closure(3);

    for (size_t i = 0; i < vetor.size(); i++)
    {
        std::cout << vetor[i] << " ";
    }

    delete graph;
    //delete ArvoreMin;

    */

//output/png/