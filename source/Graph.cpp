#include "../include/Graph.hpp"

// Tratar todos os caso de pesos nos nós;

Graph::Graph(std::ifstream& instance, bool directed, bool weighted_edges, bool weighted_nodes)
{
    this->_number_of_nodes = 0;
    this->_number_of_edges = 0;
    this->_directed        = directed;
    this->_weighted_edges  = weighted_edges;
    this->_weighted_nodes  = weighted_nodes;
    this->_first           = nullptr;
    this->_last            = nullptr;

    size_t node_id_1, node_id_2;
    float  edge_weight;

    float       number_of_nodes;
    std::string line;

    instance >> number_of_nodes;

    for (int i = 1; i <= number_of_nodes; i++)
    {
        // adicionar um metodo para adicionar um nó com peso
        this->add_node(i);
    }

    if (weighted_edges)
    {
        while (std::getline(instance, line))
        {
            std::stringstream ss(line);
            if (ss >> node_id_1 >> node_id_2 >> edge_weight)
            {
                this->add_edge(node_id_1, node_id_2, edge_weight);
            }
        }
    }
    else
    {
        while (std::getline(instance, line))
        {
            std::stringstream ss(line);
            if (ss >> node_id_1 >> node_id_2)
            {
                this->add_edge(node_id_1, node_id_2, 1);
            }
        }
    }
}

Graph::Graph(bool directed, bool weighted_edges, bool weighted_nodes)
{
    this->_number_of_nodes = 0;
    this->_number_of_edges = 0;
    this->_directed        = directed;
    this->_weighted_edges  = weighted_edges;
    this->_weighted_nodes  = weighted_nodes;
    this->_first           = nullptr;
    this->_last            = nullptr;
}

Graph::~Graph()
{
    Node *node = this->_first;
    while (node != nullptr)
    {
        Node *next_node = node->_next_node;
        Edge *edge      = node->_first_edge;
        while (edge != nullptr)
        {
            Edge *next_edge = edge->_next_edge;
            delete edge;
            edge = next_edge;
        }
        delete node;
        node = next_node;
    }
}

void Graph::remove_node(size_t node_position)
{
    Node *node = this->_first;
    while (node != nullptr)
    {
        if (node->_id == node_position)
        {
            Node *previous_node = node->_previous_node;
            Node *next_node     = node->_next_node;
            if (previous_node != nullptr)
            {
                previous_node->_next_node = next_node;
            }
            else
            {
                this->_first = next_node;
            }
            if (next_node != nullptr)
            {
                next_node->_previous_node = previous_node;
            }
            else
            {
                this->_last = previous_node;
            }
            Edge *edge = node->_first_edge;
            while (edge != nullptr)
            {
                Edge *next_edge = edge->_next_edge;
                delete edge;
                edge = next_edge;
            }
            delete node;
            this->_number_of_nodes--;
            return;
        }
        node = node->_next_node;
    }
    return;
}

void Graph::remove_edge(size_t node_position_1, size_t node_position_2)
{
    if (!this->conected(node_position_1, node_position_2))
        return;

    Node *node_1   = this->find_node(node_position_1);
    Edge *aux_edge = node_1->_first_edge;

    if (aux_edge->_target_id == node_position_2)
    {
        node_1->_first_edge = aux_edge->_next_edge;
        delete aux_edge;
        if(!this->_directed)
        {
            this->remove_edge(node_position_2, node_position_1);
        }
        node_1->_number_of_edges--;
        this->_number_of_edges--;
        return;
    }

    while (aux_edge->_next_edge->_target_id != node_position_2)
        aux_edge = aux_edge->_next_edge;

    Edge *aux_edge_2     = aux_edge->_next_edge;
    aux_edge->_next_edge = aux_edge_2->_next_edge;
    delete aux_edge_2;
    if(!this->_directed)
    {
        this->remove_edge(node_position_2, node_position_1);
    }
    node_1->_number_of_edges--;
    this->_number_of_edges--;
}

void Graph::add_node(size_t node_id, float weight)
{
    if (this->find_node(node_id) != nullptr)
        return;
    
    Node *node = this->create_node(node_id, weight);

    if (this->_first == nullptr)
    {
        this->_first = this->_last = node;
    }
    else
    {
        node->_previous_node    = this->_last;
        this->_last->_next_node = node;
        this->_last             = node;
    }

    this->_number_of_nodes++;
}

void Graph::add_edge(size_t node_id_1, size_t node_id_2, float weight, bool reverse)
{   /*
    if (this->conected(node_id_1, node_id_2))
        return;
    */

    if(find_node(node_id_2) == nullptr)
    {
        this->add_node(node_id_2);
    }

    Edge *new_edge = this->create_edge(node_id_2, weight);
    Node *node_1   = this->find_node(node_id_1);

    if(node_1 == nullptr)
    {
        this->add_node(node_id_1);
        node_1 = this->find_node(node_id_1);
    }


    Edge *aux_edge = node_1->_first_edge;

    if (aux_edge == nullptr)
    {
        node_1->_first_edge = new_edge;
        if(!this->_directed && !reverse)
        {
            this->add_edge(node_id_2, node_id_1, weight, true);
        }
    }
    else
    {
        while (aux_edge->_next_edge != nullptr)
            aux_edge = aux_edge->_next_edge;

        aux_edge->_next_edge = new_edge;
        if(!this->_directed && !reverse)
        {
            this->add_edge(node_id_2, node_id_1, weight, true);
        }
    }

    node_1->_number_of_edges++;
    this->_number_of_edges++;
}

// Qual format para imprimir no terminal ?

// Imprimindo a lista de adjacencia
void Graph::print_graph()
{
    Node *node = this->_first;

    while (node != nullptr)
    {
        std::cout << "Node " << node->_id << " : ";
        Edge *edge = node->_first_edge;
        while (edge != nullptr)
        {
            std::cout << edge->_target_id << ",  ";
            edge = edge->_next_edge;
        }
        std::cout << std::endl;
        node = node->_next_node;
    }
    return;
}

void Graph::print_graph(std::ofstream& output_file)
{
    if (this->_directed)
    {
        output_file << "digraph G {" << std::endl;
        output_file.flush();
        Node *node = this->_first;
        while (node != nullptr)
        {
            output_file << "      " << node->_id << ";" << std::endl;
            Edge *edge = node->_first_edge;
            while (edge != nullptr)
            {
                if (!this->_weighted_edges)
                {
                    output_file << "      " << node->_id << " -> " << edge->_target_id << ";" << std::endl;
                }
                else
                {
                    output_file << "      " << node->_id << " -> " << edge->_target_id << " [label=\"" << edge->_weight << "\"];" << std::endl;
                }
                edge = edge->_next_edge;
            }
            node = node->_next_node;
        }
        output_file << "}" << std::endl;
    }
    else
    {
        output_file << "graph G {" << std::endl;
        Node                               *node = this->_first;
        std::set<std::pair<size_t, size_t>> printed_edges;
        while (node != nullptr)
        {
            Edge *edge = node->_first_edge;
            while (edge != nullptr)
            {
                if (printed_edges.find({ edge->_target_id, node->_id }) == printed_edges.end())
                {
                    if (!this->_weighted_edges)
                    {
                        output_file << "      " << node->_id << " -- " << edge->_target_id << ";" << std::endl;
                    }
                    else
                    {
                        output_file << "      " << node->_id << " -- " << edge->_target_id << " [label=\"" << edge->_weight << "\"];" << std::endl;
                    }

                    printed_edges.insert({ node->_id, edge->_target_id });
                }
                else
                {
                    printed_edges.erase({ edge->_target_id, node->_id });
                }
                edge = edge->_next_edge;
            }
            node = node->_next_node;
        }
        output_file << "}" << std::endl;
    }

    return;
}

bool Graph::conected(size_t node_id_1, size_t node_id_2)
{
    Node *aux_node = find_node(node_id_1);

    if (aux_node == nullptr)
    {
        return false;
    }

    Edge *edge = aux_node->_first_edge;
    while (edge != nullptr)
    {
        if (edge->_target_id == node_id_2)
        {
            return true;
        }
        edge = edge->_next_edge;
    }

    return false;
}

int Graph::get_number_of_nodes()
{
    return this->_number_of_nodes;
}

int Graph::get_number_of_edges()
{
    return this->_number_of_edges;
}

// To do : Organizar a função DFS, criar um vetor para armazenar a arvore. Criar a funcao que salva o arquivo;

void Graph::busca_prof(size_t node_id, std::ofstream& output_file)
{
    if (this->_directed)
    {
        output_file << "digraph G {" << std::endl;
    }
    else
    {
        output_file << "graph G {" << std::endl;
    }

    std::map<size_t, bool> visited;
    Node                  *node = this->_first;
    while (node != nullptr)
    {
        visited[node->_id] = false;
        node               = node->_next_node;
    }

    std::set<std::pair<size_t, size_t>> printed_edges;
    DFS(find_node(node_id), node_id, -1, visited, output_file, printed_edges);

    // Devo fazer a busca a partir de todos os nós, caso o grafo não seja conexo ?

    /*
    node = this->_first;

    while (node != nullptr)
    {
        if (!visited[node->_id])
        {
            DFS(node, node->_id, -1, visited, output_file, printed_edges);
        }
        node = node->_next_node;
    }
    */

    output_file << "}" << std::endl;

    output_file.close();

    return;
}

void Graph ::DFS(Node *node, size_t node_id, size_t parent, std::map<size_t, bool>& visited, std::ofstream& output_file,
                 std::set<std::pair<size_t, size_t>>& printed_edges)
{
    visited[node_id] = true;
    Edge *edge       = node->_first_edge;
    while (edge != nullptr)
    {
        if (!visited[edge->_target_id])
        {
            if (this->_directed)
            {
                output_file << "      " << node_id << " -> " << edge->_target_id << " [label=\"" << edge->_weight << "\"];" << std::endl;
            }
            else
            {
                output_file << "      " << node_id << " -- " << edge->_target_id << " [label=\"" << edge->_weight << "\"];" << std::endl;
            }
            DFS(find_node(edge->_target_id), edge->_target_id, node_id, visited, output_file, printed_edges);
        }
        else if (edge->_target_id != parent)
        {
            if (this->_directed)
            {
                output_file << "      " << node_id << " -> " << edge->_target_id << " [label=\"" << edge->_weight << "\" color=\"red\" style = dashed];"
                            << std::endl;
            }
            else
            {
                if (printed_edges.find({ edge->_target_id, node_id }) == printed_edges.end())
                {
                    output_file << "      " << node_id << " -- " << edge->_target_id << " [label=\"" << edge->_weight << "\" color=\"red\" style = dashed];"
                                << std::endl;
                    printed_edges.insert({ node_id, edge->_target_id });
                }
            }
        }
        edge = edge->_next_edge;
    }
}

Graph *Graph ::kruscal(std::vector<size_t> nodes_ids)
{
    Graph                                   *subgraph = new Graph(this->_directed, this->_weighted_edges, this->_weighted_nodes);
    std::vector<std::tuple<int, int, float>> edges;
    induced_subgraph(nodes_ids, edges);
    std::sort(edges.begin(), edges.end(), [](std::tuple<int, int, float> a, std::tuple<int, int, float> b) { return std::get<2>(a) < std::get<2>(b); });

    std::map<int, int> components;

    for (size_t i : nodes_ids)
    {
        components[i] = -1;
    }

    for (int i = 0; i < edges.size(); i++)
    {
        int x = search(components, std::get<0>(edges[i]));
        int y = search(components, std::get<1>(edges[i]));

        if (x != y)
        {
            subgraph->add_edge(std::get<0>(edges[i]), std::get<1>(edges[i]), std::get<2>(edges[i]));
            Union(components, x, y);
        }
    }

    return subgraph;
}

void Graph ::induced_subgraph(std::vector<size_t> nodes_ids, std::vector<std::tuple<int, int, float>>& edges)
{
    for (int i = 0; i < nodes_ids.size(); i++)
    {
        Node *node = this->_first;
        while (node != nullptr)
        {
            if (node->_id == nodes_ids[i])
            {
                Edge *edge = node->_first_edge;
                while (edge != nullptr)
                {
                    for (int j = 0; j < nodes_ids.size(); j++)
                    {
                        if (edge->_target_id == nodes_ids[j])
                        {
                            edges.push_back({ node->_id, edge->_target_id, edge->_weight });
                        }
                    }
                    edge = edge->_next_edge;
                }
            }
            node = node->_next_node;
        }
    }

    std::cout << "------- SUBGRAFO VERTICE INDUZIDO ------- " << std::endl;

    for (int i = 0; i < edges.size(); i++)
    {
        std::cout << std::get<0>(edges[i]) << " " << std::get<1>(edges[i]) << " " << std::get<2>(edges[i]) << std::endl;
    }
}

int Graph ::search(std::map<int, int>& components, int i)
{
    if (components[i] == -1)
    {
        return i;
    }
    return search(components, components[i]);
}

void Graph ::Union(std::map<int, int>& components, int x, int y)
{
    int xset         = search(components, x);
    int yset         = search(components, y);
    components[xset] = yset;
}

std::vector<size_t> Graph::transitive_closure(size_t node_id)
{
    if (!this->_directed)
    {
        std::cout << "O grafo não é direcionado" << std::endl;
        return {};
    }

    std::vector<size_t>    stack;
    std::map<size_t, bool> visited;
    Node                  *first_node = this->_first;
    while (first_node != nullptr)
    {
        visited[first_node->_id] = false;
        first_node               = first_node->_next_node;
    }

    DFS_TC(node_id, visited, stack);

    return stack;
}

void Graph::DFS_TC(size_t node_id, std::map<size_t, bool>& visited, std::vector<size_t>& stack)
{
    visited[node_id] = true;
    stack.push_back(node_id);
    Node *node = find_node(node_id);
    Edge *edge = node->_first_edge;
    while (edge != nullptr)
    {
        if (!visited[edge->_target_id])
        {
            DFS_TC(edge->_target_id, visited, stack);
        }
        edge = edge->_next_edge;
    }
}

std::vector<size_t> Graph::transitive_indirect(size_t node_id)
{
    if (!this->_directed)
    {
        std::cout << "O grafo não é direcionado" << std::endl;
        return {};
    }

    Graph *reverse_graph = new Graph(this->_directed, this->_weighted_edges, this->_weighted_nodes);

    Node *node = this->_first;

    while (node != nullptr)
    {
        Edge *edge = node->_first_edge;
        while (edge != nullptr)
        {
            reverse_graph->add_edge(edge->_target_id, node->_id, edge->_weight);
            edge = edge->_next_edge;
        }
        node = node->_next_node;
    }

    std::vector<size_t> vetor = reverse_graph->transitive_closure(node_id);

    delete reverse_graph;

    return vetor;
}

Node* Graph::create_node(size_t node_id, float weight)
{
    Node *node             = new Node;
    node->_id              = node_id;
    node->_weight          = weight;
    node->_number_of_edges = 0;
    node->_first_edge      = nullptr;
    node->_next_node       = nullptr;
    node->_previous_node   = nullptr;

    return node;
}

Node *Graph::find_node(size_t node_id)
{
    Node *aux_node = this->_first;

    while (aux_node != nullptr && aux_node->_id != node_id)
        aux_node = aux_node->_next_node;

    return aux_node;
}

Edge *Graph::create_edge(size_t target_id, float weight)
{
    Edge *edge = new Edge;

    edge->_target_id = target_id;
    edge->_weight    = weight;
    edge->_next_edge = nullptr;

    return edge;
}