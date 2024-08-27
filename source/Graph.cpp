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
        if (!this->_directed)
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
    if (!this->_directed)
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
{ /*
    if (this->conected(node_id_1, node_id_2))
        return;
    */

    if (find_node(node_id_2) == nullptr)
    {
        this->add_node(node_id_2);
    }

    Edge *new_edge = this->create_edge(node_id_2, weight);
    Node *node_1   = this->find_node(node_id_1);

    if (node_1 == nullptr)
    {
        this->add_node(node_id_1);
        node_1 = this->find_node(node_id_1);
    }

    Edge *aux_edge = node_1->_first_edge;

    if (aux_edge == nullptr)
    {
        node_1->_first_edge = new_edge;
        if (!this->_directed && !reverse)
        {
            this->add_edge(node_id_2, node_id_1, weight, true);
        }
    }
    else
    {
        while (aux_edge->_next_edge != nullptr)
            aux_edge = aux_edge->_next_edge;

        aux_edge->_next_edge = new_edge;
        if (!this->_directed && !reverse)
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

void Graph::busca_prof(size_t node_id, std::ofstream& output_file)
{
    std::map<size_t, bool> visited;
    Node                  *node = this->_first;
    while (node != nullptr)
    {
        visited[node->_id] = false;
        node               = node->_next_node;
    }

    std::set<std::pair<size_t, size_t>>            printed_return_edges;
    std::vector<std::tuple<size_t, size_t, float>> arv;
    std::vector<std::tuple<size_t, size_t, float>> retorno;
    DFS(find_node(node_id), node_id, -1, visited, output_file, printed_return_edges, arv, retorno);

    // Devo fazer a busca a partir de todos os nós, caso o grafo não seja conexo ?

    node = this->_first;

    while (node != nullptr)
    {
        if (!visited[node->_id])
        {
            DFS(node, node->_id, -1, visited, output_file, printed_return_edges, arv, retorno);
        }
        node = node->_next_node;
    }

    vectorToDotFile(output_file, &arv, &retorno);

    output_file.close();

    return;
}

void Graph ::DFS(Node *node, size_t node_id, size_t parent, std::map<size_t, bool>& visited, std::ofstream& output_file,
                 std::set<std::pair<size_t, size_t>>& printed_return_edges, std::vector<std::tuple<size_t, size_t, float>>& arv,
                 std::vector<std::tuple<size_t, size_t, float>>& retorno)
{
    visited[node_id] = true;
    Edge *edge       = node->_first_edge;
    while (edge != nullptr)
    {
        if (!visited[edge->_target_id])
        {
            arv.push_back({ node_id, edge->_target_id, edge->_weight });
            DFS(find_node(edge->_target_id), edge->_target_id, node_id, visited, output_file, printed_return_edges, arv, retorno);
        }
        else if (edge->_target_id != parent)
        {
            if (this->_directed)
            {
                retorno.push_back({ node_id, edge->_target_id, edge->_weight });
            }
            else
            {
                if (printed_return_edges.find({ edge->_target_id, node_id }) == printed_return_edges.end())
                {
                    retorno.push_back({ node_id, edge->_target_id, edge->_weight });
                    printed_return_edges.insert({ node_id, edge->_target_id });
                }
                else
                {
                    printed_return_edges.erase({ edge->_target_id, node_id });
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

Node *Graph::create_node(size_t node_id, float weight)
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

void Graph ::vectorToDotFile(std::ofstream& output_file, std::vector<std::tuple<size_t, size_t, float>> *arvore,
                             std::vector<std::tuple<size_t, size_t, float>> *arestas_retorno)
{
    if (this->_directed)
    {
        output_file << "digraph G {" << std::endl;
        output_file.flush();
        for (auto edge : *arvore)
        {
            output_file << "      " << std::get<0>(edge) << " -> " << std::get<1>(edge) << " [label=\"" << std::get<2>(edge) << "\"];" << std::endl;
        }
    }
    else
    {
        output_file << "graph G {" << std::endl;
        for (auto edge : *arvore)
        {
            output_file << "      " << std::get<0>(edge) << " -- " << std::get<1>(edge) << " [label=\"" << std::get<2>(edge) << "\"];" << std::endl;
        }
    }

    if (arestas_retorno != nullptr)
    {
        if (this->_directed)
        {
            for (auto edge : *arestas_retorno)
            {
                output_file << "      " << std::get<0>(edge) << " -> " << std::get<1>(edge) << " [label=\"" << std::get<2>(edge)
                            << "\", color=\"red\" style = dashed];" << std::endl;
            }
        }
        else
        {
            for (auto edge : *arestas_retorno)
            {
                output_file << "      " << std::get<0>(edge) << " -- " << std::get<1>(edge) << " [label=\"" << std::get<2>(edge)
                            << "\", color=\"red\" style = dashed];" << std::endl;
            }
        }
    }

    output_file << "}" << std::endl;

    return;
}

std::vector<std::vector<float>> Graph::create_matrix()
{
    std::vector<std::vector<float>> matrix(this->_number_of_nodes, std::vector<float>(this->_number_of_nodes, std::numeric_limits<float>::infinity()));

    Node *node = this->_first;
    while (node != nullptr)
    {
        matrix[node->_id - 1][node->_id - 1] = 0;
        Edge *edge                           = node->_first_edge;
        while (edge != nullptr)
        {
            matrix[node->_id - 1][edge->_target_id - 1] = edge->_weight;
            edge                                        = edge->_next_edge;
        }
        node = node->_next_node;
    }

    return matrix;
}

std::vector<std::vector<float>> Graph::create_path_matrix()
{
    std::vector<std::vector<float>> matrix(this->_number_of_nodes, std::vector<float>(this->_number_of_nodes));

    Node *node = this->_first;
    while (node != nullptr)
    {
        matrix[node->_id - 1][node->_id - 1] = -1;
        Edge *edge                           = node->_first_edge;
        while (edge != nullptr)
        {
            matrix[node->_id - 1][edge->_target_id - 1] = node->_id - 1;
            edge                                        = edge->_next_edge;
        }
        node = node->_next_node;
    }

    return matrix;
}

std::vector<size_t> Graph::floyd_warshall(size_t node_id_1, size_t node_id_2)
{
    std::vector<std::vector<float>> matrix      = create_matrix();
    std::vector<std::vector<float>> path_matrix = create_path_matrix();
    std::vector<size_t>             path;

    for (int k = 0; k < this->_number_of_nodes; k++)
    {
        for (int i = 0; i < this->_number_of_nodes; i++)
        {
            for (int j = 0; j < this->_number_of_nodes; j++)
            {
                if (matrix[i][j] > matrix[i][k] + matrix[k][j])
                {
                    matrix[i][j]      = matrix[i][k] + matrix[k][j];
                    path_matrix[i][j] = path_matrix[k][j];
                }
            }
        }
    }

    int i = node_id_1 - 1;
    int j = node_id_2 - 1;

    if (matrix[i][j] == std::numeric_limits<float>::infinity())
    {
        return path;
    }

    while (i != j)
    {
        path.push_back(j + 1);
        j = path_matrix[i][j];
    }

    path.push_back(i + 1);

    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<size_t> Graph::edsger_dijkstra(size_t node_id_1, size_t node_id_2)
{
    std::vector<std::vector<float>> matrix      = create_matrix();
    std::vector<std::vector<float>> path_matrix = create_path_matrix();
    std::vector<size_t>             path;
    
    int i = node_id_1 - 1;
    int j = node_id_2 - 1;

    if (matrix[i][j] == std::numeric_limits<float>::infinity())
    {
        return path;
    }

    // algortimo

    return path;
}


std::vector<std::vector<float>> Graph::distancias_minimas()
{
    std::vector<std::vector<float>> matrix = create_matrix();

    for (int k = 0; k < this->_number_of_nodes; k++)
    {
        for (int i = 0; i < this->_number_of_nodes; i++)
        {
            for (int j = 0; j < this->_number_of_nodes; j++)
            {
                if (matrix[i][j] > matrix[i][k] + matrix[k][j])
                {
                    matrix[i][j] = matrix[i][k] + matrix[k][j];
                }
            }
        }
    }

    return matrix;
}

std::vector<float> Graph::get_excentricidades()
{
    std::vector<std::vector<float>> matrix = distancias_minimas();
    int                             n      = matrix.size();
    std::vector<float>              excentricidades(n, 0);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i != j)
            {
                if (excentricidades[i] == 0)
                {
                    excentricidades[i] = matrix[i][j];
                }
                else
                {
                    excentricidades[i] = std::max(excentricidades[i], matrix[i][j]);
                }
            }
        }
    }

    return excentricidades;
}

float Graph::get_diametro()
{
    std::vector<float> excentricidades = get_excentricidades();
    return *std::max_element(excentricidades.begin(), excentricidades.end());
}

float Graph::get_raio()
{
    std::vector<float> excentricidades = get_excentricidades();
    return *std::min_element(excentricidades.begin(), excentricidades.end());
}

std::vector<size_t> Graph::get_centro()
{
    std::vector<float>  excentricidades = get_excentricidades();
    float               raio            = get_raio();
    std::vector<size_t> centro;

    for (int i = 0; i < excentricidades.size(); i++)
    {
        if (excentricidades[i] == raio)
        {
            centro.push_back(i + 1);
        }
    }

    return centro;
}

std::vector<size_t> Graph::get_periferia()
{
    std::vector<float>  excentricidades = get_excentricidades();
    float               diametro        = get_diametro();
    std::vector<size_t> periferia;

    for (int i = 0; i < excentricidades.size(); i++)
    {
        if (excentricidades[i] == diametro)
        {
            periferia.push_back(i + 1);
        }
    }

    return periferia;
}


Graph* Graph::prim(size_t start_node_id) {
    if (this->_first == nullptr) {
        return nullptr;
    }

    Graph* mst = new Graph(this->_directed, this->_weighted_edges, this->_weighted_nodes);

    std::map<size_t, bool> visited;
    Node* current = this->_first;
    while (current != nullptr) {
        visited[current->_id] = false;
        current = current->_next_node;
    }

    std::priority_queue<std::tuple<float, size_t, size_t>,
    std::vector<std::tuple<float, size_t, size_t>>,
    std::greater<std::tuple<float, size_t, size_t>>> pq;

    visited[start_node_id] = true;
    Node* start_node = this->find_node(start_node_id);
    Edge* edge = start_node->_first_edge;

    while (edge != nullptr) {
        pq.push(std::make_tuple(edge->_weight, start_node_id, edge->_target_id));
        edge = edge->_next_edge;
    }

    while (!pq.empty()) {
        auto [weight, from_node, to_node] = pq.top();
        pq.pop();

        if (visited[to_node]) {
            continue;
        }

        visited[to_node] = true;

        mst->add_edge(from_node, to_node, weight);

        Node* next_node = this->find_node(to_node);
        Edge* next_edge = next_node->_first_edge;

        while (next_edge != nullptr) {
            if (!visited[next_edge->_target_id]) {
                pq.push(std::make_tuple(next_edge->_weight, to_node, next_edge->_target_id));
            }
            next_edge = next_edge->_next_edge;
        }
    }

    return mst;
}
