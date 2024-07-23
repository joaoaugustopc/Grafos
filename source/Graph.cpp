#include "../include/Graph.hpp"

Graph::Graph(std::ifstream& instance)
{
    this->_number_of_nodes = 0;
    this->_number_of_edges = 0;
    this->_directed = true;
    this->_weighted_edges = true;
    this->_weighted_nodes = false;
    this->_first = nullptr;
    this->_last = nullptr;
    
    // Uma outra maneira seria criar todos os nós, com os respectivos labels e depois adicionar as arestas.
    std::string nodes;
    std::getline(instance,nodes);
    int number_of_nodes = std::stoi(nodes);
    

    std::string line;
    std::vector<std::tuple<int,int,float>> edges;

    while(std::getline(instance,line))
    {
        int node1, node2;
        float weight;
        std::stringstream ss(line);
        if(ss >> node1 >> node2 >> weight)
        {
            edges.push_back({node1,node2,weight});
        }
    }

    instance.close();

    for(const auto& edge : edges)
    {
        int node1, node2;
        float weight;
        std::tie(node1,node2,weight) = edge;
        add_edge(node1,node2,weight);
    }

    return;

}

Graph::Graph()
{
    this->_number_of_nodes = 0;
    this->_number_of_edges = 0;
    this->_directed = false;
    this->_weighted_edges = false;
    this->_weighted_nodes = false;
    this->_first = nullptr;
    this->_last = nullptr;
}

Graph::~Graph()
{
    Node *node = this->_first;
    while (node != nullptr){
        Node *next_node = node->_next_node;
        Edge *edge = node->_first_edge;
        while (edge != nullptr){
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
    Node *new_node = new Node;
    new_node->_number_of_edges = 0;
    new_node->_id = node_id;
    new_node->_weight = weight;
    new_node->_first_edge = nullptr;

    if (this->_number_of_nodes == 0){
        this->_first = new_node;
        this->_last = new_node;
        new_node->_next_node = nullptr;
        new_node->_previous_node = nullptr;
        this->_number_of_nodes++;
        return;
    }
    else if (this->_number_of_nodes == 1){
        this->_first->_next_node = new_node;
        new_node->_previous_node = this->_first;
        new_node->_next_node = nullptr;
        this->_last = new_node;
        this->_number_of_nodes++;
        return;
    }
    else {
        this->_last->_next_node = new_node;
        new_node->_previous_node = this->_last;
        new_node->_next_node = nullptr;
        this->_last = new_node;
        this->_number_of_nodes++;
        return;
    }

}

//Se não existir algum dos nós, ele cria o nó que não existe
void Graph::add_edge(size_t node_id_1, size_t node_id_2, float weight)
{
    if(find_node(node_id_2) == nullptr){
        add_node(node_id_2);
    }
    

    Edge *new_edge = new Edge;
    this->_number_of_edges++;
    new_edge->_weight = weight;
    new_edge->_target_id = node_id_2;
    new_edge->_next_edge = nullptr;

    Node *node = this->_first;
    while (node != nullptr){
        if (node->_id == node_id_1){
            if (node->_first_edge == nullptr){
                node->_first_edge = new_edge;
                node->_number_of_edges++;
                return;
            }
            else {
                Edge *edge = node->_first_edge;
                while (edge->_next_edge != nullptr){
                    edge = edge->_next_edge;
                }
                edge->_next_edge = new_edge;
                node->_number_of_edges++;
                return;
            }
        }
        node = node->_next_node;
    }

    add_node(node_id_1);
    this->_last->_first_edge = new_edge;
    this->_last->_number_of_edges++;
    return;
}

void Graph::print_graph()
{
    
}

void Graph::print_graph(std::ofstream& output_file)
{
    output_file << "digraph G {" << std::endl;
    Node *node = this->_first;
    while (node != nullptr){
        Edge *edge = node->_first_edge;
        while (edge != nullptr){
            output_file << "      " << node->_id << " -> " << edge->_target_id << " [label=\"" << edge->_weight << "\"];" << std::endl;
            edge = edge->_next_edge;
        }
        node = node->_next_node;
    }
    output_file << "}" << std::endl;
    return;    
}

int Graph::conected(size_t node_id_1, size_t node_id_2)
{
    Node *node = this->_first;
    while (node != nullptr){
        if (node->_id == node_id_1){
            Edge *edge = node->_first_edge;
            while (edge != nullptr){
                if (edge->_target_id == node_id_2){
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

Node* Graph::find_node(size_t node_id)
{
    Node *node = this->_first;
    while (node != nullptr){
        if (node->_id == node_id){
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
