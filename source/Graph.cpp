#include "../include/Graph.hpp"

Graph::Graph(std::ifstream& instance, bool directed, bool weighted_edges, bool weighted_nodes)
{
    this->_number_of_nodes = 0;
    this->_number_of_edges = 0;
    this->_directed        = directed;
    this->_weighted_edges  = weighted_edges;
    this->_weighted_nodes  = weighted_nodes;
    this->_first           = nullptr;
    this->_last            = nullptr;

    // Uma outra maneira seria criar todos os nós, com os respectivos labels e depois adicionar as arestas.
    std::string nodes;
    std::getline(instance, nodes);
    int number_of_nodes = std::stoi(nodes);

    std::string                              line;
    std::vector<std::tuple<int, int, float>> edges;

    if (!weighted_edges)
    {
        while (std::getline(instance, line))
        {
            int               node1, node2;
            std::stringstream ss(line);
            if (ss >> node1 >> node2)
            {
                edges.push_back({ node1, node2, 0 });
            }
        }
    }

    else
    {
        while (std::getline(instance, line))
        {
            int               node1, node2;
            float             weight;
            std::stringstream ss(line);
            if (ss >> node1 >> node2 >> weight)
            {
                edges.push_back({ node1, node2, weight });
            }
        }
    }

    instance.close();

    for (const auto& edge : edges)
    {
        int   node1, node2;
        float weight;
        std::tie(node1, node2, weight) = edge;
        add_edge(node1, node2, weight);
    }

    return;
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
}

void Graph::remove_edge(size_t node_position_1, size_t node_position_2)
{
}

void Graph::add_node(size_t node_id, float weight)
{
    Node *new_node             = new Node;
    new_node->_number_of_edges = 0;
    new_node->_id              = node_id;
    new_node->_weight          = weight;
    new_node->_first_edge      = nullptr;

    if (this->_number_of_nodes == 0)
    {
        this->_first             = new_node;
        this->_last              = new_node;
        new_node->_next_node     = nullptr;
        new_node->_previous_node = nullptr;
        this->_number_of_nodes++;
        return;
    }
    else if (this->_number_of_nodes == 1)
    {
        this->_first->_next_node = new_node;
        new_node->_previous_node = this->_first;
        new_node->_next_node     = nullptr;
        this->_last              = new_node;
        this->_number_of_nodes++;
        return;
    }
    else
    {
        this->_last->_next_node  = new_node;
        new_node->_previous_node = this->_last;
        new_node->_next_node     = nullptr;
        this->_last              = new_node;
        this->_number_of_nodes++;
        return;
    }
}

//Se não existir algum dos nós, ele cria o nó que não existe
void Graph::add_edge(size_t node_id_1, size_t node_id_2, float weight, bool reverse)
{
    if (find_node(node_id_2) == nullptr)
    {
        add_node(node_id_2);
    }

    Edge *new_edge = new Edge;
    this->_number_of_edges++;
    new_edge->_weight    = weight;
    new_edge->_target_id = node_id_2;
    new_edge->_next_edge = nullptr;

    Node *node = this->_first;
    while (node != nullptr)
    {
        if (node->_id == node_id_1)
        {
            if (node->_first_edge == nullptr)
            {
                node->_first_edge = new_edge;
                node->_number_of_edges++;
                if (!this->_directed && !reverse)
                {
                    add_edge(node_id_2, node_id_1, weight, true);
                }
                return;
            }
            else
            {
                Edge *edge = node->_first_edge;
                while (edge->_next_edge != nullptr)
                {
                    edge = edge->_next_edge;
                }
                edge->_next_edge = new_edge;
                node->_number_of_edges++;
                if (!this->_directed && !reverse)
                {
                    add_edge(node_id_2, node_id_1, weight, true);
                }
                return;
            }
        }
        node = node->_next_node;
    }

    add_node(node_id_1);
    this->_last->_first_edge = new_edge;
    this->_last->_number_of_edges++;

    if (!this->_directed && !reverse)
    {
        add_edge(node_id_2, node_id_1, weight, true);
    }

    return;
}

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
        Node *node = this->_first;
        while (node != nullptr)
        {
            Edge *edge = node->_first_edge;
            while (edge != nullptr)
            {
                output_file << "      " << node->_id << " -> " << edge->_target_id << " [label=\"" << edge->_weight << "\"];" << std::endl;
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
                    output_file << "      " << node->_id << " -- " << edge->_target_id << " [label=\"" << edge->_weight << "\"];" << std::endl;
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

int Graph::conected(size_t node_id_1, size_t node_id_2)
{
    Node *node = this->_first;
    while (node != nullptr)
    {
        if (node->_id == node_id_1)
        {
            Edge *edge = node->_first_edge;
            while (edge != nullptr)
            {
                if (edge->_target_id == node_id_2)
                {
                    return edge->_weight;
                }
                edge = edge->_next_edge;
            }
            return -1;
        }
        node = node->_next_node;
    }
    return -1;
}

Node *Graph::find_node(size_t node_id)
{
    Node *node = this->_first;
    while (node != nullptr)
    {
        if (node->_id == node_id)
        {
            return node;
        }
        node = node->_next_node;
    }
    return nullptr;
}

int Graph::get_number_of_nodes()
{
    return this->_number_of_nodes;
}

int Graph::get_number_of_edges()
{
    return this->_number_of_edges;
}

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

    node = this->_first;

    while (node != nullptr)
    {
        if (!visited[node->_id])
        {
            DFS(node, node->_id, -1, visited, output_file, printed_edges);
        }
        node = node->_next_node;
    }

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

void Graph :: kruscal(std::vector<size_t> nodes_ids, std::ofstream& output_file)
{
    Graph* subgraph = new Graph(this->_directed, this->_weighted_edges, this->_weighted_nodes);
    std::vector<std::tuple<int, int, float>> edges;
    induced_subgraph(nodes_ids, edges);
    std::sort(edges.begin(), edges.end(), [](std::tuple<int, int, float> a, std::tuple<int, int, float> b) {
        return std::get<2>(a) < std::get<2>(b);
    });

    std::map<int, int> components;

    for (size_t i : nodes_ids)
    {
        components[i] = -1;
    }


    for(int i = 0; i < edges.size(); i++){
        int x = search(components, std::get<0>(edges[i]));
        int y = search(components, std::get<1>(edges[i]));

        if(x != y){
            subgraph->add_edge(std::get<0>(edges[i]), std::get<1>(edges[i]), std::get<2>(edges[i]));
            Union(components, x, y);   
        }

    }
    
    subgraph->print_graph(output_file);
    delete subgraph;
}

void Graph :: induced_subgraph(std::vector<size_t> nodes_ids, std::vector<std::tuple<int, int, float>>& edges)
{ 
    for(int i = 0; i < nodes_ids.size(); i++)
    {
        Node *node = this->_first;
        while(node != nullptr)
        {
            if(node->_id == nodes_ids[i])
            {
                Edge *edge = node->_first_edge;
                while(edge != nullptr)
                {
                    for(int j = 0; j < nodes_ids.size(); j++)
                    {
                        if(edge->_target_id == nodes_ids[j])
                        {
                            edges.push_back({node->_id, edge->_target_id, edge->_weight});
                        }
                    }
                    edge = edge->_next_edge;
                }
            }
            node = node->_next_node;
        }
    }

    for(int i = 0; i < edges.size(); i++)
    {
        std::cout << std::get<0>(edges[i]) << " " << std::get<1>(edges[i]) << " " << std::get<2>(edges[i]) << std::endl;
    }

    std::cout<<"ok" << std::endl;
    
}

int Graph :: search(std::map<int, int>& components, int i)
{
    if(components[i] == -1)
    {
        return i;
    }
    return search(components, components[i]);
}

void Graph :: Union(std::map<int, int>&components, int x, int y)
{
    int xset = search(components, x);
    int yset = search(components, y);
    components[xset] = yset;
}