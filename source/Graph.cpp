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

    int                              p, num_nodes;
    std::vector<int>                 nodes;
    std::map<int, int>               nodes_weight;
    std::vector<std::pair<int, int>> edges;
    std::string                      skip;
    std::string                      line;

    while (std::getline(instance, line))
    {
        if (line.find("param p := ") != std::string::npos)
        {
            std::stringstream ss(line);
            ss >> skip >> skip >> skip >> p;
            std::cout << "Particoes: " << p << std::endl;
        }

        if (line.find("vertici") != std::string::npos)
        {
            std::stringstream ss(line);
            ss >> skip >> num_nodes;
            std::cout << "Numero de vertices: " << num_nodes << std::endl;
        }

        if (line.find("set V :=") != std::string::npos)
        {
            std::cout << "Vertices: " << std::endl;
            while (std::getline(instance, line))
            {
                if (line.find(";") != std::string::npos)
                {
                    break;
                }
                //std::cout << line << std::endl;
                std::stringstream ss(line);

                while (ss >> skip)
                {
                    nodes.push_back(std::stoi(skip));
                }
            }
        }

        if (line.find("param w :=") != std::string::npos)
        {
            std::cout << "Pesos: " << std::endl;
            while (std::getline(instance, line))
            {
                if (line.find(";") != std::string::npos)
                {
                    break;
                }
                std::stringstream ss(line);
                int               u, w;
                ss >> u >> w;
                nodes_weight[u] = w;
            }
        }

        if (line.find("set E :=") != std::string::npos)
        {
            std::cout << "Arestas: " << std::endl;
            while (std::getline(instance, line))
            {
                if (line.find(";") != std::string::npos)
                {
                    break;
                }
                std::stringstream ss(line);
                char              ignore;
                int               u, v;
                while (ss >> ignore >> u >> ignore >> v >> ignore)
                {
                    edges.push_back({ u, v });
                }
            }
        }
    }

    for (int i = 0; i < num_nodes; i++)
    {
        int node_id = nodes[i];
        this->add_node(node_id, nodes_weight[node_id]);
    }

    for (auto i : edges)
    {
        this->add_edge(i.first, i.second, 1);
    }

    this->_number_of_components = p;

    /*
    size_t node_id_1, node_id_2;
    float  edge_weight;

    float       number_of_nodes;
    std::string line;

    instance >> number_of_nodes;

    for (int i = 1; i <= number_of_nodes; i++)
    {
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

    */
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
{
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
            output_file << "      " << node->_id << "[label=\"" << node->_weight << "\"];" << std::endl;
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

    for (size_t i = 0; i < edges.size(); i++)
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
    for (size_t i = 0; i < nodes_ids.size(); i++)
    {
        Node *node = this->_first;
        while (node != nullptr)
        {
            if (node->_id == nodes_ids[i])
            {
                Edge *edge = node->_first_edge;
                while (edge != nullptr)
                {
                    for (size_t j = 0; j < nodes_ids.size(); j++)
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

    for (size_t i = 0; i < edges.size(); i++)
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

    for (size_t k = 0; k < this->_number_of_nodes; k++)
    {
        for (size_t i = 0; i < this->_number_of_nodes; i++)
        {
            for (size_t j = 0; j < this->_number_of_nodes; j++)
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
    std::map<size_t, float>                                                                                                      dist;
    std::map<size_t, size_t>                                                                                                     previous;
    std::vector<size_t>                                                                                                          path;
    std::priority_queue<std::pair<float, size_t>, std::vector<std::pair<float, size_t>>, std::greater<std::pair<float, size_t>>> queue;

    for (Node *node = _first; node != nullptr; node = node->_next_node)
    {
        dist[node->_id]     = std::numeric_limits<float>::infinity();
        previous[node->_id] = (size_t)-1;  // Use size_t para representar o valor inválido
    }

    dist[node_id_1] = 0;
    queue.push({ 0, node_id_1 });

    while (!queue.empty())
    {
        size_t u = queue.top().second;
        queue.pop();

        if (u == node_id_2)
        {
            break;
        }

        Node *current_node = find_node(u);
        for (Edge *edge = current_node->_first_edge; edge != nullptr; edge = edge->_next_edge)
        {
            size_t v   = edge->_target_id;
            float  alt = dist[u] + edge->_weight;

            if (alt < dist[v])
            {
                dist[v]     = alt;
                previous[v] = u;
                queue.push({ alt, v });
            }
        }
    }

    for (size_t at = node_id_2; at != (size_t)-1; at = previous[at])
    {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());

    if (path.size() == 1 && path[0] != node_id_1)
    {
        path.clear();
    }

    return path;
}

std::vector<std::vector<float>> Graph::distancias_minimas()
{
    std::vector<std::vector<float>> matrix = create_matrix();

    for (size_t k = 0; k < this->_number_of_nodes; k++)
    {
        for (size_t i = 0; i < this->_number_of_nodes; i++)
        {
            for (size_t j = 0; j < this->_number_of_nodes; j++)
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

    for (size_t i = 0; i < excentricidades.size(); i++)
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

    for (size_t i = 0; i < excentricidades.size(); i++)
    {
        if (excentricidades[i] == diametro)
        {
            periferia.push_back(i + 1);
        }
    }

    return periferia;
}

Graph *Graph::prim(size_t start_node_id)
{
    Graph *mst = new Graph(this->_directed, this->_weighted_edges, this->_weighted_nodes);

    if (!this->_weighted_edges)
    {
        std::cerr << "O algoritmo de Prim requer um grafo com arestas ponderadas." << std::endl;
        return mst;
    }

    std::set<size_t> visited;

    auto compare = [](const std::tuple<size_t, size_t, float>& a, const std::tuple<size_t, size_t, float>& b) { return std::get<2>(a) > std::get<2>(b); };

    std::priority_queue<std::tuple<size_t, size_t, float>, std::vector<std::tuple<size_t, size_t, float>>, decltype(compare)> min_heap(compare);

    visited.insert(start_node_id);

    Node *start_node = this->find_node(start_node_id);
    Edge *edge       = start_node->_first_edge;
    while (edge != nullptr)
    {
        min_heap.push(std::make_tuple(start_node_id, edge->_target_id, edge->_weight));
        edge = edge->_next_edge;
    }

    while (!min_heap.empty())
    {
        std::tuple<size_t, size_t, float> edge   = min_heap.top();
        size_t                            node1  = std::get<0>(edge);
        size_t                            node2  = std::get<1>(edge);
        float                             weight = std::get<2>(edge);
        min_heap.pop();

        if (visited.find(node2) == visited.end())
        {
            mst->add_edge(node1, node2, weight);
            visited.insert(node2);
            Node *new_node = this->find_node(node2);
            Edge *new_edge = new_node->_first_edge;
            while (new_edge != nullptr)
            {
                if (visited.find(new_edge->_target_id) == visited.end())
                {
                    min_heap.push(std::make_tuple(node2, new_edge->_target_id, new_edge->_weight));
                }
                new_edge = new_edge->_next_edge;
            }
        }
    }

    return mst;
}

void Graph::DFS_ArticulationPoints(int node_id, std::map<int, bool>& visited, std::map<int, int>& discoveryTime, std::map<int, int>& lowTime,
                                   std::map<int, int>& parent, std::vector<int>& articulationPoints, int& time)
{
    visited[node_id]       = true;
    discoveryTime[node_id] = lowTime[node_id] = ++time;
    int childCount                            = 0;

    Node *node = this->find_node(node_id);
    for (Edge *edge = node->_first_edge; edge != nullptr; edge = edge->_next_edge)
    {
        int neighbor_id = edge->_target_id;

        if (!visited[neighbor_id])
        {
            childCount++;
            parent[neighbor_id] = node_id;
            DFS_ArticulationPoints(neighbor_id, visited, discoveryTime, lowTime, parent, articulationPoints, time);

            lowTime[node_id] = std::min(lowTime[node_id], lowTime[neighbor_id]);

            if (parent[node_id] == -1 && childCount > 1)
            {
                articulationPoints.push_back(node_id);
            }
            if (parent[node_id] != -1 && lowTime[neighbor_id] >= discoveryTime[node_id])
            {
                articulationPoints.push_back(node_id);
            }
        }
        else if (neighbor_id != parent[node_id])
        {
            lowTime[node_id] = std::min(lowTime[node_id], discoveryTime[neighbor_id]);
        }
    }
}

std::vector<int> Graph::findArticulationPoints()
{
    std::vector<int>    articulationPoints;
    std::map<int, int>  discoveryTime;
    std::map<int, int>  lowTime;
    std::map<int, bool> visited;
    std::map<int, int>  parent;
    int                 time = 0;

    for (Node *node = _first; node != nullptr; node = node->_next_node)
    {
        visited[node->_id]       = false;
        discoveryTime[node->_id] = -1;
        lowTime[node->_id]       = -1;
        parent[node->_id]        = -1;
    }

    for (Node *node = _first; node != nullptr; node = node->_next_node)
    {
        if (!visited[node->_id])
        {
            DFS_ArticulationPoints(node->_id, visited, discoveryTime, lowTime, parent, articulationPoints, time);
        }
    }

    return articulationPoints;
}

// fauxs
void Graph ::initialize_tabu_matrix(std::map<int, std::map<int, int>>& tabuMatrix, int numVertices, int numSubgraphs)
{
    // Inicializa a matriz tabu com zeros
    for (int v = 0; v < numVertices + 1; ++v)
    {
        for (int s = 0; s < numSubgraphs + 1; ++s)
        {
            tabuMatrix[v][s] = -std::numeric_limits<int>::max();
        }
    }
}

bool Graph ::is_articulation_vertex(int v, const std::vector<int>& subgraph, const std::vector<std::vector<int>>& adjList)
{
    // Criar um conjunto dos vértices do subgrafo sem o vértice v
    std::unordered_set<int> subgraph_set(subgraph.begin(), subgraph.end());
    subgraph_set.erase(v);

    if (subgraph_set.size() < 2)
    {
        return false;
    }

    // Iniciar DFS a partir de um vértice qualquer do subgrafo
    int                     start_vertex = *subgraph_set.begin();
    std::unordered_set<int> visited;
    std::stack<int>         stack;
    stack.push(start_vertex);
    visited.insert(start_vertex);

    while (!stack.empty())
    {
        int current = stack.top();
        stack.pop();

        for (int neighbor : adjList[current])
        {
            if (neighbor == v)
                continue;
            if (subgraph_set.find(neighbor) != subgraph_set.end())
            {
                if (visited.find(neighbor) == visited.end())
                {
                    visited.insert(neighbor);
                    stack.push(neighbor);
                }
            }
        }
    }

    // Se o número de visitados é igual ao tamanho do subgrafo sem v, não é articulação
    return visited.size() != subgraph_set.size();
}

std::vector<std::tuple<int, int, int>>
Graph ::generate_neighborhood(const std::vector<std::vector<int>>& adjList, const std::vector<std::vector<int>>& current_solution)
{
    std::vector<std::tuple<int, int, int>> moves;
    int                                    numSubgraphs = current_solution.size();

    for (int s_from = 0; s_from < numSubgraphs; ++s_from)
    {
        const auto& subgraph_from = current_solution[s_from];
        for (int v : subgraph_from)
        {
            // Encontrar subgrafos adjacentes
            for (int s_to = 0; s_to < numSubgraphs; ++s_to)
            {
                if (s_to == s_from)
                    continue;
                const auto& subgraph_to = current_solution[s_to];
                bool        is_adjacent = false;
                for (int u : subgraph_to)
                {
                    if (std::find(adjList[v].begin(), adjList[v].end(), u) != adjList[v].end())
                    {
                        is_adjacent = true;
                        break;
                    }
                }
                if (is_adjacent)
                {
                    // Ajuste da condição para tamanho mínimo do subgrafo de origem
                    if (subgraph_from.size() - 1 >= 3)
                    {
                        // Verificar se v não é vértice de articulação
                        if (!is_articulation_vertex(v, subgraph_from, adjList))
                        {
                            moves.push_back(std::make_tuple(v, s_from, s_to));
                        }
                    }
                }
            }
        }
    }

    return moves;
}

bool Graph ::is_tabu(const std::tuple<int, int, int>& move, const std::map<int, std::map<int, int>>& tabuMatrix, int iteration, int l_in)
{
    int v    = std::get<0>(move);
    int s_to = std::get<2>(move);

    if (tabuMatrix.at(v).at(s_to) + l_in > iteration)
    {
        return true;  // Movimento tabu
    }
    return false;  // Movimento permitido
}

void Graph ::evaluate_moves(const std::vector<std::tuple<int, int, int>>& neighborhood_moves, const std::vector<std::vector<int>>& current_solution,
                            const std::map<int, std::map<int, int>>& tabuMatrix, int iteration, int l_in, const std::vector<std::vector<int>>& best_solution,
                            std::tuple<int, int, int>& best_move, double& best_gap, const std::vector<int>& vertexWeights)
{
    double aspiration_value = compute_total_gap(best_solution, vertexWeights);
    for (const auto& move : neighborhood_moves)
    {
        int v      = std::get<0>(move);
        int s_from = std::get<1>(move);
        int s_to   = std::get<2>(move);

        bool tabu = is_tabu(move, tabuMatrix, iteration, l_in);

        std::vector<std::vector<int>> new_solution = current_solution;

        new_solution[s_from].erase(std::remove(new_solution[s_from].begin(), new_solution[s_from].end(), v), new_solution[s_from].end());
        new_solution[s_to].push_back(v);

        double total_gap = compute_total_gap(new_solution, vertexWeights);

        // Verifica se o movimento é permitido ou satisfaz o critério do aspiration_value
        if (!tabu || (tabu && total_gap < aspiration_value))
        {
            if (total_gap < best_gap)
            {
                best_move = move;
                best_gap  = total_gap;
            }
        }
    }
}

void Graph ::apply_move(const std::tuple<int, int, int>& move, std::vector<std::vector<int>>& current_solution)
{
    int v      = std::get<0>(move);
    int s_from = std::get<1>(move);
    int s_to   = std::get<2>(move);

    // Remove o vértice do subgrafo de origem
    current_solution[s_from].erase(std::remove(current_solution[s_from].begin(), current_solution[s_from].end(), v), current_solution[s_from].end());

    // Adiciona o vértice ao subgrafo de destino
    current_solution[s_to].push_back(v);
}

double Graph ::compute_total_gap(const std::vector<std::vector<int>>& solution, const std::vector<int>& vertexWeights)
{
    double total_gap = 0.0;
    for (const auto& subgraph : solution)
    {
        if (!subgraph.empty())
        {
            int max_weight = std::numeric_limits<int>::min();
            int min_weight = std::numeric_limits<int>::max();
            for (int v : subgraph)
            {
                int weight = vertexWeights[v];
                if (weight > max_weight)
                    max_weight = weight;
                if (weight < min_weight)
                    min_weight = weight;
            }
            total_gap += (max_weight - min_weight);
        }
    }
    return total_gap;
}

void Graph ::update_tabu_matrix(std::map<int, std::map<int, int>>& tabuMatrix, const std::tuple<int, int, int>& move, int iteration)
{
    int v    = std::get<0>(move);
    int s_to = std::get<2>(move);

    // Atualiza a tabu
    tabuMatrix[v][s_to] = iteration;
}

//Finalmente a função Tabu Search!!
std::vector<std::vector<int>> Graph ::tabu_search(const std::vector<std::vector<int>>& adjList, const std::vector<int>& vertexWeights,
                                                  const std::vector<std::vector<int>>& initial_partition, int max_iter, int l_min, int l_max)
{
    // Solução inicial
    std::vector<std::vector<int>>     current_solution = initial_partition;
    std::vector<std::vector<int>>     best_solution    = current_solution;
    std::map<int, std::map<int, int>> tabuMatrix;
    int                               numVertices  = vertexWeights.size();
    int                               numSubgraphs = current_solution.size();
    initialize_tabu_matrix(tabuMatrix, numVertices, numSubgraphs);
    int    l_in                  = l_max;
    double previous_solution_gap = compute_total_gap(current_solution, vertexWeights);

    for (int iteration = 0; iteration < max_iter; ++iteration)
    {
        std::cout << "Iteração " << iteration << std::endl;
        // Explorar a vizinhança
        std::vector<std::tuple<int, int, int>> neighborhood_moves = generate_neighborhood(adjList, current_solution);

        std::cout << "teste1" << std::endl;

        // Avaliar cada movimento
        std::tuple<int, int, int> best_move(-1, -1, -1);
        double                    best_gap = std::numeric_limits<double>::max();

        evaluate_moves(neighborhood_moves, current_solution, tabuMatrix, iteration, l_in, best_solution, best_move, best_gap, vertexWeights);
        std::cout << "teste2" << std::endl;

        // Verifica se um movimento válido foi encontrado
        if (std::get<0>(best_move) == -1)
        {
            // Nenhum movimento válido disponível
            break;
        }

        // Aplicar o melhor movimento encontrado
        apply_move(best_move, current_solution);

        // Atualizar a matriz tabu
        update_tabu_matrix(tabuMatrix, best_move, iteration);
        std::cout << "teste3" << std::endl;

        // Atualizar a melhor solução
        double current_solution_gap = compute_total_gap(current_solution, vertexWeights);
        if (current_solution_gap < compute_total_gap(best_solution, vertexWeights))
        {
            best_solution = current_solution;
        }

        if (current_solution_gap < previous_solution_gap)
        {
            l_in = std::max(l_in - 1, l_min);  // Se houve melhoria, intensificar a busca
        }
        else
        {
            l_in = std::min(l_in + 1, l_max);  // Caso contrário, diversificar a busca
        }
        previous_solution_gap = current_solution_gap;
    }

    return best_solution;
}

void Graph ::mggp()
{

    int p = this->_number_of_components;
    std::cout << "Executando o algoritmo MGGP com p = " << p << std::endl;

    std::vector<Graph *> solution = constructive_procedure(p);

    for (Graph *partition : solution)
    {
        std::cout << "Subgrafo: ";
        for (Node *node = partition->_first; node != nullptr; node = node->_next_node)
        {
            std::cout << node->_id << " ";
        }
        std::cout << std::endl;
    }

    std::vector<int> vertexWeights(this->_number_of_nodes + 1, 0);
    std::cout << "1" << std::endl;

    for (Node *node = this->_first; node != nullptr; node = node->_next_node)
    {
        vertexWeights[node->_id] = node->_weight;
    }

    std::vector<std::vector<int>> initial_partition;

    std::cout << "2" << std::endl;

    for (Graph *partition : solution)
    {
        std::vector<int> subgraph;

        std::vector<Node> nodes = partition->get_nodes();

        for (Node node : nodes)
        {
            std::cout << "Node: " << node._id << std::endl;
            subgraph.push_back(node._id);
        }

        initial_partition.push_back(subgraph);
    }

    std::cout << "3" << std::endl;
    std::vector<std::vector<int>> adjList = create_adjacency_list();

    std::cout << "4" << std::endl;

    std::vector<std::vector<int>> best_solution = tabu_search(adjList, vertexWeights, initial_partition, 1000, 5, 10);

    std::cout << "Solução final encontrada" << std::endl;

    std::cout << "Melhor solução encontrada: " << std::endl;
    for (int i = 0; i < best_solution.size(); ++i)
    {
        std::cout << "Subgrafo " << i + 1 << ": ";
        for (int v : best_solution[i])
        {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Gap total: " << compute_total_gap(best_solution, vertexWeights) << std::endl;

    for (Graph *partition : solution)
    {
        delete partition;
    }
}

// begin: Construção de Soluções

std::vector<std::tuple<int, int>> Graph ::get_crescent_gap_edges(std::vector<int>& nodes, std::map<int, int>& nodes_weight)
{
    //Ordenar as arestas em ordem crescente de gap e armazenar em um vetor
    std::vector<std::tuple<int, int>> edges;
    std::set<std::pair<int, int>>     seen_edges;
    for (Node *node = this->_first; node != nullptr; node = node->_next_node)
    {
        nodes.push_back(node->_id);
        nodes_weight[node->_id] = node->_weight;

        Edge *edge = node->_first_edge;
        while (edge != nullptr)
        {
            int u = node->_id;
            int v = edge->_target_id;

            // Garantir que a aresta seja sempre representada como (menor, maior)
            if (u > v)
                std::swap(u, v);

            // Verificar se essa aresta já foi inserida
            if (seen_edges.find({ u, v }) == seen_edges.end())
            {
                edges.push_back({ u, v });
                seen_edges.insert({ u, v });  // Marcar a aresta como processada
            }

            edge = edge->_next_edge;
        }
    }

    //Ordenar as arestas em ordem crescente de gap | w(u) - w(v) |
    std::sort(edges.begin(),
              edges.end(),
              [&](const std::tuple<int, int>& a, const std::tuple<int, int>& b)
              {
                  return std::abs(nodes_weight.at(std::get<0>(a)) - nodes_weight.at(std::get<1>(a))) <
                         std::abs(nodes_weight.at(std::get<0>(b)) - nodes_weight.at(std::get<1>(b)));
              });

    return edges;
}

std::vector<Graph *> Graph ::constructive_procedure(int p)
{
    std::cout << "Executando o procedimento construtivo com p = " << p << std::endl;
    //Vetor de Subgrafos (Partições)
    std::vector<int>   nodes;
    std::map<int, int> nodes_weight;  // Mapa de pesos dos vértices (chave: id do vértice, valor: peso do vértice)

    std::vector<std::tuple<int, int>> edges = get_crescent_gap_edges(nodes, nodes_weight);

    // Inicializar o conjunto de partições
    //Conjunto Solução
    std::vector<Graph *> X;
    std::vector<int>     added_nodes;  // Conjunto de vértices adicionados a alguma partição

    int edge_index = 0;
    int i          = 0;
    // Inicializar as partições
    while (X.size() < p && edge_index < edges.size())
    {
        // extrai a primeira aresta de edges (O menor gap)
        std::tuple<int, int> edge = edges[edge_index++];

        // Verificar se algum vertice da aresta não é adjacente a nenhuma partição
        if (std::find(added_nodes.begin(), added_nodes.end(), std::get<0>(edge)) == added_nodes.end() &&
            std::find(added_nodes.begin(), added_nodes.end(), std::get<1>(edge)) == added_nodes.end())
        {
            std::cout << "Criando particao " << i << std::endl;
            i = i + 1;
            // Criar uma nova partição
            Graph *partition = new Graph(this->_directed, this->_weighted_edges, this->_weighted_nodes);
            partition->add_node(std::get<0>(edge), nodes_weight[std::get<0>(edge)]);
            partition->add_node(std::get<1>(edge), nodes_weight[std::get<1>(edge)]);
            partition->add_edge(std::get<0>(edge), std::get<1>(edge), 1);
            X.push_back(partition);
            added_nodes.push_back(std::get<0>(edge));
            added_nodes.push_back(std::get<1>(edge));
        }
    }

    for (auto partition : X)
    {
        std::cout << "Partição: ";
        for (Node *node = partition->_first; node != nullptr; node = node->_next_node)
        {
            std::cout << node->_id << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Partições iniciais criadas" << std::endl;

    //Expandir os subconjuntos até que todos os vértices estejam em uma partição
    while (added_nodes.size() < this->_number_of_nodes)
    {
        std::cout << "Adicionando vértices às partições:" << std::endl;
        int                  melhor_gap = std::numeric_limits<int>::max();
        std::tuple<int, int> best_move;
        Graph               *target_partition = nullptr;

        //para cada edge [i,j] em E tal que [i,j] conecta um vértice de X com um vértice fora de X
        for (auto& edge : edges)
        {
            //std::cout << "Adicionando vértice " << std::get<0>(edge) << " ou " << std::get<1>(edge) << " às partições" << std::endl;
            bool v_in = std::find(added_nodes.begin(), added_nodes.end(), std::get<0>(edge)) != added_nodes.end();
            bool u_in = std::find(added_nodes.begin(), added_nodes.end(), std::get<1>(edge)) != added_nodes.end();

            if (v_in != u_in)
            {
                int v = v_in ? std::get<0>(edge) : std::get<1>(edge);
                int u = v_in ? std::get<1>(edge) : std::get<0>(edge);

                //Simular a adição de u à partição que contém v
                for (auto partition : X)
                {
                    if (partition->find_node(v) != nullptr)
                    {
                        //Simular adição
                        partition->add_node(u, nodes_weight[u]);
                        int gap = compute_total_gap(X, nodes_weight);

                        //Verificar se o gap é melhor
                        if (gap < melhor_gap)
                        {
                            melhor_gap       = gap;
                            best_move        = edge;
                            target_partition = partition;
                        }

                        //Remover nó adicionado
                        partition->remove_node(u);
                        break;
                    }
                }
            }
        }

        std::cout << "Melhor gap encontrado: " << melhor_gap << std::endl;

        //Aplicar o melhor movimento encontrado
        int v = target_partition->find_node(std::get<0>(best_move)) ? std::get<0>(best_move) : std::get<1>(best_move);
        int u = v == std::get<0>(best_move) ? std::get<1>(best_move) : std::get<0>(best_move);

        target_partition->add_node(u, nodes_weight[u]);
        target_partition->add_edge(v, u, 1);
        added_nodes.push_back(u);
    }

    std::cout << "SAIU" << std::endl;
    return X;
}

int Graph ::compute_gap(std::map<int, int> partition_weights)
{
    int max_weight = std::numeric_limits<int>::min();
    int min_weight = std::numeric_limits<int>::max();

    for (auto i : partition_weights)
    {
        if (i.second > max_weight)
        {
            max_weight = i.second;
        }
        if (i.second < min_weight)
        {
            min_weight = i.second;
        }
    }

    return max_weight - min_weight;
}

int Graph ::compute_total_gap(std::vector<Graph *>& partitions, std::map<int, int>& node_weights)
{
    int total_gap = 0;
    for (const auto partition : partitions)
    {
        std::map<int, int> partition_weights;
        std::vector<Node>  nodes = partition->get_nodes();
        for (const auto node : nodes)
        {
            partition_weights[node._id] = node._weight;
        }

        total_gap += partition->compute_gap(partition_weights);
    }
    return total_gap;
}

// end: Construção de Soluções

std::vector<Node> Graph ::get_nodes()
{
    std::vector<Node> nodes;
    for (Node *node = _first; node != nullptr; node = node->_next_node)
    {
        nodes.push_back(*node);
    }
    return nodes;
}

std::map<int, int> Graph ::get_partition_weights(Graph& partition)
{
    std::map<int, int> partition_weights;
    std::vector<Node>  nodes = partition.get_nodes();
    for (auto node : nodes)
    {
        partition_weights[node._id] = node._weight;
    }
    return partition_weights;
}
/*
double Graph::compute_total_gap(const std::vector<std::vector<int>>& partitions, const std::map<int, float>& node_weights)
{
    double total_gap = 0.0;

    for (const auto& partition : partitions)
    {
        if (partition.size() >= 2)
        {
            float max_weight = std::numeric_limits<float>::min();
            float min_weight = std::numeric_limits<float>::max();

            for (int node_id : partition)
            {
                float weight = node_weights.at(node_id);
                if (weight > max_weight) max_weight = weight;
                if (weight < min_weight) min_weight = weight;
            }

            total_gap += (max_weight - min_weight);
        }
        else
        {
            // Penalizar partições com menos de dois nós
            total_gap += std::numeric_limits<float>::max();
        }
    }

    return total_gap;
}*/

/*bool Graph::is_connected_subgraph(const std::vector<int>& subgraph_nodes)
{
    if (subgraph_nodes.size() < 2)
        return false;

    std::unordered_set<int> visited;
    std::queue<int> q;
    q.push(subgraph_nodes[0]);
    visited.insert(subgraph_nodes[0]);

    while (!q.empty())
    {
        int current = q.front();
        q.pop();

        Node* node = find_node(current);
        Edge* edge = node->_first_edge;

        while (edge != nullptr)
        {
            int neighbor_id = edge->_target_id;

            if (std::find(subgraph_nodes.begin(), subgraph_nodes.end(), neighbor_id) != subgraph_nodes.end())
            {
                if (visited.find(neighbor_id) == visited.end())
                {
                    visited.insert(neighbor_id);
                    q.push(neighbor_id);
                }
            }
            edge = edge->_next_edge;
        }
    }

    return visited.size() == subgraph_nodes.size();
}*/

/*void Graph::local_search(std::vector<std::vector<int>>& partitions, const std::map<int, float>& node_weights)
{
    bool improved = true;

    while (improved)
    {
        improved = false;

        // Para cada partição
        for (size_t i = 0; i < partitions.size(); ++i)
        {
            // Tentar mover um nó para outra partição
            for (size_t j = 0; j < partitions.size(); ++j)
            {
                if (i == j) continue;

                for (int node_id : partitions[i])
                {
                    // Verificar se a remoção do nó não desconecta a partição
                    if (can_remove_node(partitions[i], node_id))
                    {
                        // Verificar se a adição do nó à partição j mantém a conectividade
                        if (can_add_node(partitions[j], node_id))
                        {
                            // Calcular o gap atual e o novo gap se a mudança for feita
                            double current_gap = compute_total_gap_GRASP(partitions, node_weights);

                            // Realizar a mudança
                            partitions[i].erase(std::remove(partitions[i].begin(), partitions[i].end(), node_id), partitions[i].end());
                            partitions[j].push_back(node_id);

                            double new_gap = compute_total_gap_GRASP(partitions, node_weights);

                            if (new_gap < current_gap)
                            {
                                improved = true;
                                break;
                            }
                            else
                            {
                                // Reverter a mudança
                                partitions[j].pop_back();
                                partitions[i].push_back(node_id);
                            }
                        }
                    }
                }
                if (improved) break;
            }
            if (improved) break;
        }
    }
}
*/

std::vector<std::vector<int>> Graph::reativo(int max_iter, float alpha)
{
    int p = this->_number_of_components;
    std::vector<int> nodes;
    std::map<int, float> node_weights;

    // Coletar os nós e seus pesos
    for (Node* node = this->_first; node != nullptr; node = node->_next_node)
    {
        nodes.push_back(node->_id);
        node_weights[node->_id] = node->_weight;
    }

    std::vector<std::vector<int>> best_solution;
    double best_gap = std::numeric_limits<double>::max();

    for (int iter = 0; iter < max_iter; ++iter)
    {
        std::cout << "Iteracao " << iter << std::endl;
        // Fase de Construção
        std::vector<std::vector<int>> solution = constructive_phase(nodes, node_weights, p, alpha);
        std::cout << "teste1" << std::endl;

        // Fase de Busca Local
        local_search(solution, node_weights);
        std::cout << "teste2" << std::endl;

        // Avaliar a solução
        double total_gap = compute_total_gap_GRASP(solution, node_weights);
        std::cout << "teste3" << std::endl;

        if (total_gap < best_gap)
        {
            best_gap = total_gap;
            best_solution = solution;
        }
    }

    int total_gap = compute_total_gap_GRASP(best_solution, node_weights);
        std::cout << "teste4" << std::endl;


    std::cout << "Melhor solução encontrada: " << std::endl;
    for (int i = 0; i < best_solution.size(); ++i)
    {
        std::cout << "Subgrafo " << i + 1 << ": ";
        for (int v : best_solution[i])
        {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Gap total: " << total_gap << std::endl;


    return best_solution;
}


std::vector<std::vector<int>> Graph::create_adjacency_list()
{
    std::cout << "Criando lista de adjacência" << std::endl;
    std::vector<std::vector<int>> adjList(this->_number_of_nodes + 1);
    for (Node *node = this->_first; node != nullptr; node = node->_next_node)
    {
        Edge *edge = node->_first_edge;
        while (edge != nullptr)
        {
            adjList[node->_id].push_back(edge->_target_id);
            edge = edge->_next_edge;
        }
    }

    std::cout << "Lista de adjacência criada" << std::endl;
    return adjList;
}

///algoritmo GRASP

//Construção

std::vector<std::tuple<int, int>> Graph::build_RCL(const std::vector<std::tuple<int, int>>& edges,
                                            const std::map<int, float>& node_weights, float alpha)
{
    std::vector<std::tuple<int, int>> RCL;

    if (edges.empty())
        return RCL;

    float min_gap = std::abs(node_weights.at(std::get<0>(edges.front())) - node_weights.at(std::get<1>(edges.front())));
    float max_gap = std::abs(node_weights.at(std::get<0>(edges.back())) - node_weights.at(std::get<1>(edges.back())));

    float threshold = min_gap + alpha * (max_gap - min_gap);

    for (const auto& edge : edges)
    {
        float gap = std::abs(node_weights.at(std::get<0>(edge)) - node_weights.at(std::get<1>(edge)));
        if (gap <= threshold)
        {
            RCL.push_back(edge);
        }
        else
        {
            break;
        }
    }

    return RCL;
}


std::vector<std::tuple<int, int>> Graph::get_edges_ordered_by_gap(const std::map<int, float>& node_weights)
{
    std::vector<std::tuple<int, int>> edges;

    for (Node* node = this->_first; node != nullptr; node = node->_next_node)
    {
        Edge* edge = node->_first_edge;
        while (edge != nullptr)
        {
            int u = node->_id;
            int v = edge->_target_id;

            if (!_directed && u > v)
            {
                edge = edge->_next_edge;
                continue;
            }

            edges.push_back({u, v});
            edge = edge->_next_edge;
        }
    }

    // Ordenar as arestas em ordem crescente de gap | w(u) - w(v) |
    std::sort(edges.begin(), edges.end(), [&](const std::tuple<int, int>& a, const std::tuple<int, int>& b) {
        float gap_a = std::abs(node_weights.at(std::get<0>(a)) - node_weights.at(std::get<1>(a)));
        float gap_b = std::abs(node_weights.at(std::get<0>(b)) - node_weights.at(std::get<1>(b)));
        return gap_a < gap_b;
    });

    return edges;
}

int Graph::select_random_unassigned_node(const std::vector<int>& nodes,
                                        const std::unordered_set<int>& added_nodes)
{
    std::vector<int> unassigned_nodes;

    for (int node_id : nodes)
    {
        if (added_nodes.find(node_id) == added_nodes.end())
        {
            unassigned_nodes.push_back(node_id);
        }
    }

    if (unassigned_nodes.empty())
    {
        return -1;
    }

    int random_index = rand() % unassigned_nodes.size();
    return unassigned_nodes[random_index];
}

std::vector<std::vector<int>> Graph::constructive_phase(const std::vector<int>& nodes,
                                    const std::map<int, float>& node_weights, int p, float alpha)
{
    std::vector<std::vector<int>> partitions(p); // Inicializa 'p' partições vazias
    std::unordered_set<int> added_nodes;

    std::vector<std::tuple<int, int>> edges = get_edges_ordered_by_gap(node_weights);

    //std::cout <<"Teste antes while - constructive_phase" << std::endl;
    
    // Enquanto houver nós não adicionados
    while (added_nodes.size() < nodes.size())
    {
    //std::cout <<"Teste while - constructive_phase" << std::endl;
        // Construir a RCL
        std::vector<std::tuple<int, int>> RCL = build_RCL(edges, node_weights, alpha);

        // Selecionar aleatoriamente uma aresta da RCL
        if (!RCL.empty())
        {
            // Adicionar um nó não atribuído a uma partição aleatória
            int node_id = select_random_unassigned_node(nodes, added_nodes);
            //std::cout <<"Dentro IF - constructive_phase" << std::endl;    
            int partition_index = rand() % p;
            partitions[partition_index].push_back(node_id);
            added_nodes.insert(node_id);
            continue;
        }

        auto edge = RCL[rand() % RCL.size()];
        int u = std::get<0>(edge);
        int v = std::get<1>(edge);

        // Determinar em qual partição adicionar os nós
        // Implementação da lógica para adicionar os nós às partições garantindo as restrições
        // ...

        // Após modificar 'partitions', se precisar avaliar o gap:
        // double total_gap = compute_total_gap_GRASP(partitions, node_weights);
        // Use 'total_gap' conforme necessário
    }

    return partitions;
}


//GRASP fauxs

bool Graph::is_connected_subgraph(const std::vector<int>& subgraph_nodes)
{
    if (subgraph_nodes.size() < 2)
        return false;

    std::unordered_set<int> visited;
    std::queue<int> q;

    int start_node = subgraph_nodes[0];
    q.push(start_node);
    visited.insert(start_node);

    // Converta o vetor de nós em um conjunto para acesso rápido
    std::unordered_set<int> node_set(subgraph_nodes.begin(), subgraph_nodes.end());

    while (!q.empty())
    {
        int current = q.front();
        q.pop();

        Node* node = find_node(current);
        Edge* edge = node->_first_edge;

        while (edge != nullptr)
        {
            int neighbor_id = edge->_target_id;

            // Verificar se o vizinho está no subgrafo
            if (node_set.find(neighbor_id) != node_set.end())
            {
                // Se o vizinho ainda não foi visitado
                if (visited.find(neighbor_id) == visited.end())
                {
                    visited.insert(neighbor_id);
                    q.push(neighbor_id);
                }
            }

            edge = edge->_next_edge;
        }
    }

    // O subgrafo é conectado se todos os nós foram visitados
    return visited.size() == subgraph_nodes.size();
}


bool Graph::can_remove_node(const std::vector<int>& partition, int node_id)
{
    if (partition.size() <= 2)
        return false;

    std::vector<int> subgraph_nodes = partition;
    subgraph_nodes.erase(std::remove(subgraph_nodes.begin(), subgraph_nodes.end(), node_id), subgraph_nodes.end());

    return is_connected_subgraph(subgraph_nodes);
}

bool Graph::can_add_node(const std::vector<int>& partition, int node_id)
{
    std::vector<int> subgraph_nodes = partition;
    subgraph_nodes.push_back(node_id);

    return is_connected_subgraph(subgraph_nodes);
}

double Graph::compute_total_gap_GRASP(const std::vector<std::vector<int>>& partitions,
                                const std::map<int, float>& node_weights)
{
    double total_gap = 0.0;

    for (const auto& partition : partitions)
    {
        if (partition.size() >= 2)
        {
            auto minmax = std::minmax_element(
                partition.begin(),
                partition.end(),
                [&](int a, int b) {
                    return node_weights.at(a) < node_weights.at(b);
                });

            float min_weight = node_weights.at(*minmax.first);
            float max_weight = node_weights.at(*minmax.second);

            total_gap += (max_weight - min_weight);
        }
        else
        {
            // Penalizar partições com menos de dois nós
            total_gap += std::numeric_limits<float>::max();
        }
    }

    return total_gap;
}

void Graph::local_search(std::vector<std::vector<int>>& partitions, const std::map<int, float>& node_weights)
{
    bool improved = true;

    while (improved)
    {
        improved = false;

        // Para cada partição
        for (size_t i = 0; i < partitions.size(); ++i)
        {
            // Para cada nó na partição i
            for (size_t k = 0; k < partitions[i].size(); ++k)
            {
                int node_id = partitions[i][k];

                // Verificar se a partição i permanece válida após a remoção do nó
                if (can_remove_node(partitions[i], node_id))
                {
                    // Tentar mover o nó para cada uma das outras partições
                    for (size_t j = 0; j < partitions.size(); ++j)
                    {
                        if (i == j) continue;

                        // Verificar se é possível adicionar o nó à partição j
                        if (can_add_node(partitions[j], node_id))
                        {
                            // Salvar o estado atual das partições
                            auto original_partitions = partitions;

                            // Realizar a movimentação do nó
                            partitions[i].erase(partitions[i].begin() + k);
                            partitions[j].push_back(node_id);

                            // Calcular o gap após a movimentação
                            double new_gap = compute_total_gap_GRASP(partitions, node_weights);

                            // Calcular o gap antes da movimentação
                            double current_gap = compute_total_gap_GRASP(original_partitions, node_weights);

                            // Verificar se houve melhoria
                            if (new_gap < current_gap)
                            {
                                improved = true;
                                break; // Saia do loop das partições j
                            }
                            else
                            {
                                // Reverter a movimentação
                                partitions = original_partitions;
                            }
                        }
                    }

                    if (improved)
                        break; // Saia do loop dos nós k
                }
            }

            if (improved)
                break; // Saia do loop das partições i
        }
    }
}

///Finalmente GRASP!!
std::vector<std::vector<int>> Graph::grasp(int max_iter, float alpha)
{
    int p = this->_number_of_components;
    std::vector<int> nodes;
    std::map<int, float> node_weights;

    // Coletar os nós e seus pesos
    for (Node* node = this->_first; node != nullptr; node = node->_next_node)
    {
        nodes.push_back(node->_id);
        node_weights[node->_id] = node->_weight;
    }

    std::vector<std::vector<int>> best_solution;
    double best_gap = std::numeric_limits<double>::max();

    for (int iter = 0; iter < max_iter; ++iter)
    {
        std::cout << "Iteracao " << iter << std::endl;
        // Fase de Construção
        std::vector<std::vector<int>> solution = constructive_phase(nodes, node_weights, p, alpha);
        std::cout << "teste1" << std::endl;

        // Fase de Busca Local
        local_search(solution, node_weights);
        std::cout << "teste2" << std::endl;

        // Avaliar a solução
        double total_gap = compute_total_gap_GRASP(solution, node_weights);
        std::cout << "teste3" << std::endl;

        if (total_gap < best_gap)
        {
            best_gap = total_gap;
            best_solution = solution;
        }
    }

    int total_gap = compute_total_gap_GRASP(best_solution, node_weights);
        std::cout << "teste4" << std::endl;


    std::cout << "Melhor solução encontrada: " << std::endl;
    for (int i = 0; i < best_solution.size(); ++i)
    {
        std::cout << "Subgrafo " << i + 1 << ": ";
        for (int v : best_solution[i])
        {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Gap total: " << total_gap << std::endl;


    return best_solution;
}



