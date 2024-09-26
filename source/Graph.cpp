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

void Graph::new_read(std::ifstream& instance)
{
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
            std::getline(instance, line);
            //std::cout << line << std::endl;
            std::stringstream ss(line);

            while (ss >> skip)
            {
                nodes.push_back(std::stoi(skip));
            }
        }

        if (line.find("param w :=") != std::string::npos)
        {
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

    for (auto i : nodes)
    {
        std::cout << i << " ";
    }

    std::cout << std::endl;

    for (auto i : nodes_weight)
    {
        std::cout << i.first << " " << i.second << std::endl;
    }

    for (auto i : edges)
    {
        std::cout << i.first << " " << i.second << std::endl;
    }
}
