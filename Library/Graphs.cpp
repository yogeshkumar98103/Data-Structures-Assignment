//
// Created by Yogesh Kumar on 2019-08-29.
//

#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <type_traits>
using namespace std;

static const int INF = 0x3f3f3f3f;

// ====================================================================
//                         Graph Classification
// ====================================================================

enum GraphType{
    GraphType_WeightedDirected,
    GraphType_WeightedUndirected,
    GraphType_UnweightedDirected,
    GraphType_UnweightedUndirected,
}; 

enum GraphRepresentation{
    GraphRepresentation_AdjacencyList,
    GraphRepresentation_AdjacencyMatrix,
}; 

struct unweighted_edge{
    int to;
};

template <typename weight_t>
struct weighted_edge{ 
    int to;
    weight_t weight;
};

template <typename data_t, GraphRepresentation graph_rep_t, typename edge_t, typename weight_t = void>
class Base_Graph{};

template <typename data_t, GraphType graph_t, GraphRepresentation graph_rep_t, typename weight_t = void>
class Graph{};

// ====================================================================
//                     BASE Graph Specializations
// ====================================================================

// Graph in Adjacency List
template <typename data_t, typename edge_t, typename weight_t>
class Base_Graph<data_t, GraphRepresentation_AdjacencyList, edge_t, weight_t>{
  protected:
    int vertex_count;
    vector<data_t> vertex_data;
    vector<vector<edge_t>> adj_list;

  public:
    Base_Graph(){
        this->vertex_count = 0;
    }

    int addVertex(data_t data){
        this->vertex_data.push_back(data);
        this->adj_list.push_back(vector<edge_t>());
        return this->vertex_count++;
    }

    // ----------------------- Getters --------------------------
    data_t& vertex_value(int vertex){
        return this->vertex_data[vertex];
    }

    // ---------------------- Traversal -------------------------
    void dfs(void (*call_back)(int vertex, data_t data)){
        bool visited[this->vertex_count];
        memset(visited, 0, sizeof(visited));

        for(int vertex = 0; vertex < this->vertex_count; ++vertex){
            if(!visited[vertex]){
                dfs_helper(vertex, call_back, visited);
            }
        }
    }

    void bfs(void (*call_back)(int vertex, data_t data)){
        bool visited[this->vertex_count];
        memset(visited, 0, sizeof(visited));

        for(int vertex = 0; vertex < this->vertex_count; ++vertex){
            if(!visited[vertex]){
                bfs_helper(vertex, call_back, visited);
            }
        }
    }

    // --------------------  Shortest Path ------------------------

    // One Source Multiple Destination
    template <typename U = weight_t, typename enable_if<is_same<U, void>::value>::type * = nullptr>
    pair<vector<int>, vector<int>> shortestPath(int source, int destination){
        return this->bfs_shortest_path(source);
    }

    template <typename U = weight_t, typename enable_if<!is_same<U, void>::value>::type * = nullptr>
    pair<weight_t, vector<int>> shortestPath(int source, int destination){
       return this->dijkstra(source);
    }

    // One Source One Destination
    template <typename U = weight_t, typename enable_if<is_same<U, void>::value>::type * = nullptr>
    pair<int, vector<int>> shortestPath(int source){
        auto result = this->bfs_shortest_path(source);

        vector<int> path = result.first;
        int distance = result.second[destination];
        vector<int> final_path;

        if(path[destination] == -1){
            return make_pair(0, vector<int>());
        }

        int current = destination;
        while(current != -1){
            final_path.push_back(current);
            current = path[current];
        }

        reverse(final_path.begin(), final_path.end());
        return make_pair(distance, final_path);
    }

    template <typename U = weight_t, typename enable_if<!is_same<U, void>::value>::type * = nullptr>
    pair<weight_t, vector<int>> shortestPath(int source){
        auto result = this->dijkstra(source);

        vector<int> path = result.first;
        weight_t distance = result.second[destination];
        vector<int> final_path;

        if(path[destination] == -1){
            return make_pair(0, vector<int>());
        }

        int current = destination;
        while(current != -1){
            final_path.push_back(current);
            current = path[current];
        }

        reverse(final_path.begin(), final_path.end());
        return make_pair(distance, final_path);
    }

    // Multiple Sources Multiple Destinations
    template <typename U = weight_t, typename enable_if<is_same<U, void>::value>::type * = nullptr>
    pair<vector<vector<int>>, vector<vector<int>>> allPairShortestPath(){
        vector<vector<int>> distances(this->vertex_count, vector<int>(this->vertex_count));
        vector<vector<int>> path(this->vertex_count, vector<int>(this->vertex_count));

        for(int source = 0; source < this->vertex_count; ++source){
            auto result = bfs_shortest_path(source);
            path[source] = result.first;
            distances[source] = result.second;
        }

        return make_pair(path, distances);
    }

    template <typename U = weight_t, typename enable_if<!is_same<U, void>::value>::type * = nullptr>
    pair<vector<vector<int>>, vector<vector<weight_t>>> allPairShortestPath(){
        return floyd_Warshall();
    }

    // --------------------- Helper Methods -----------------------

  protected: 
    void dfs_helper(int vertex, void (*call_back)(int vertex, data_t data), bool visited[]){
        visited[vertex] = true;
        call_back(vertex, this->vertex_data[vertex]);

        for(const edge_t& edge: this->adj_list[vertex]){
            if(!visited[edge.to]){
                dfs_helper(edge.to, call_back, visited);
            }
        }
    }

    void bfs_helper(int vertex, void (*call_back)(int vertex, data_t data), bool visited[]){
        queue<int> q;
        q.push(vertex);
        visited[vertex] = true;

        while(!q.empty()){
            vertex = q.front();
            q.pop();
            call_back(vertex, this->vertex_data[vertex]);

            for(const edge_t& edge: this->adj_list[vertex]){
                if(!visited[edge.to]){
                    q.push(edge.to);
                    visited[edge.to] = true;
                }
            }
        }
    }
 
    pair<vector<int>, vector<weight_t>> dijkstra(int source){
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> heap; 
        heap.push(make_pair(0, source)); 
        vector<int> path(this->vertex_count, -1);
        vector<weight_t> distances(this->vertex_count, INF);
        distances[source] = 0;
        path[source] = -1; 
  
        int at, to;
        while (!heap.empty()){ 
            at = heap.top().second; 
            heap.pop(); 

            for(const edge_t& edge: this->adj_list[at]){  
                to = edge.to; 
                if(distances[to] > distances[at] + edge.weight){ 
                    distances[to] = distances[at] + edge.weight; 
                    heap.push(make_pair(distances[to], to)); 
                    path[to] = at;
                } 
            }
        } 

        return make_pair(path, distances);
    } 

    pair<vector<int>, vector<int>> bfs_shortest_path(int source){
        queue<int> q;
        vector<int> path(this->vertex_count, -1);
        vector<int> distances(this->vertex_count, 0);
        bool visited[this->vertex_count];
        memset(visited, 0, sizeof(visited));

        q.push(source);
        q.push(-1);
        visited[source] = true;
        int level = 1;

        int at, to;
        while(!q.empty()){
            at = q.front();
            q.pop();

            if(at == -1){
                if(!q.empty()) q.push(-1);
                level++;
                continue;
            }

            for(const edge_t& edge: this->adj_list[at]){
                to = edge.to;
                if(!visited[to]){
                    q.push(edge.to);
                    visited[to] = true;
                    path[to] = at;
                    distances[to] = level;
                }
            }
        }
        return make_pair(path, distances);
    }

    pair<vector<vector<int>>, vector<vector<weight_t>>> floyd_Warshall(){
        vector<vector<weight_t>> distances(this->vertex_count, vector<weight_t>(this->vertex_count, INF));
        vector<vector<int>> path(this->vertex_count, vector<int>(this->vertex_count, -1));
        
        for(int vertex = 0; vertex < this->vertex_count; ++vertex)  
            for(const edge_t edge: this->adj_list[vertex])  
                distances[vertex][edge.to] = edge.weight;  

        int from,to,through;
        for(through = 0; through < this->vertex_count; ++through){
            for(from = 0; from < this->vertex_count; ++from){ 
                for(to = 0; to < this->vertex_count; ++to){    
                    if(distances[from][through] + distances[through][to] < distances[from][to]){
                        distances[from][to] = distances[from][through] + distances[through][to];
                        path[from][to] = path[from][through];
                    }    
                }  
            }  
        }

        return make_pair(path, distances);
    }  
};

// Graph in Adjacency Matrix
template <typename data_t, typename weight_t>
class Base_Graph<data_t, GraphRepresentation_AdjacencyMatrix, void, weight_t>{
  protected:
    int vertex_count;
    vector<data_t> vertex_data;
    vector<vector<bool>> adj_matrix;
    vector<vector<weight_t>> weight_matrix;

  public:
    Base_Graph(){
        vertex_count = 0;
    }

    int addVertex(data_t data){
        this->vertex_data.push_back(data);
        this->vertex_count++;
        this->adj_matrix.push_back(vector<weight_t>(this->vertex_count, false));
        this->weight_matrix.push_back(vector<weight_t>(this->vertex_count));
        return (this->vertex_count - 1);
    }
};

// ====================================================================
//                       Graph Specializations
// ====================================================================

// Weighted Undirected Graph using adjacency list
template <typename data_t, typename weight_t>
class Graph<data_t, GraphType_WeightedUndirected, GraphRepresentation_AdjacencyList, weight_t>
: public Base_Graph<data_t, GraphRepresentation_AdjacencyList, weighted_edge<weight_t>, weight_t>{
    
    typedef weighted_edge<weight_t> edge_t;
    typedef Base_Graph<data_t, GraphRepresentation_AdjacencyList, edge_t, weight_t> base_t;
    
    public: 

    Graph():base_t(){
    }

    void addEdge(int vertex1, int vertex2, weight_t weight){
        if(vertex1 > this->vertex_count || vertex2 > this->vertex_count) return;
        this->adj_list[vertex1].push_back({vertex2, weight});
        this->adj_list[vertex2].push_back({vertex1, weight});
    }
};

// Unweighted Undirected Graph using adjacency list
template <typename data_t>
class Graph<data_t, GraphType_UnweightedUndirected, GraphRepresentation_AdjacencyList, void>
: public Base_Graph<data_t, GraphRepresentation_AdjacencyList, unweighted_edge>{
    
    typedef unweighted_edge edge_t;
    typedef Base_Graph<data_t, GraphRepresentation_AdjacencyList, edge_t> base_t;
    
    public: 

    Graph():base_t(){
    }

    void addEdge(int vertex1, int vertex2){
        if(vertex1 > this->vertex_count || vertex2 > this->vertex_count) return;
        this->adj_list[vertex1].push_back({vertex2});
        this->adj_list[vertex2].push_back({vertex1});
    }
};

// Unweighted Directed Graph using adjacency list
template <typename data_t>
class Graph<data_t, GraphType_UnweightedDirected, GraphRepresentation_AdjacencyList, void>
: public Base_Graph<data_t, GraphRepresentation_AdjacencyList, unweighted_edge>{
    
    typedef unweighted_edge edge_t;
    typedef Base_Graph<data_t, GraphRepresentation_AdjacencyList, edge_t> base_t;
    
    public: 

    Graph():base_t(){
    }

    void addEdge(int from, int to){
        if(from > this->vertex_count || to > this->vertex_count) return;
        this->adj_list[from].push_back({to});
    }
};

// Weighted Directed Graph using adjacency list
template <typename data_t, typename weight_t>
class Graph<data_t, GraphType_WeightedDirected, GraphRepresentation_AdjacencyList, weight_t>
: public Base_Graph<data_t, GraphRepresentation_AdjacencyList, weighted_edge<weight_t>, weight_t>{
    
    typedef weighted_edge<weight_t> edge_t;
    typedef Base_Graph<data_t, GraphRepresentation_AdjacencyList, edge_t, weight_t> base_t;
    
    public: 

    Graph():base_t(){
    }

    void addEdge(int from, int to, weight_t weight){
        if(from > this->vertex_count || to > this->vertex_count) return;
        this->adj_list[from].push_back({to, weight});
    }
};

// TOPOLOGICAL SORT
// void topologicalSortHelper(int current, vector<bool>& visited, const vector<vector<int>>& adjList, stack<int>& vertices){
//     visited[current] = true;

//     // Visit all neighbours
//     for(int neighbour: adjList[current]){
//         if(!visited[neighbour]){
//             topologicalSortHelper(neighbour, visited, adjList, vertices);
//         }
//     }
//     vertices.push(current);
// }
// vector<int> topologicalSort(const int vertexCount, const vector<vector<int>> adjList){
//     vector<bool> visited(vertexCount, false);
//     stack<int> vertices;
//     for(int i = 1; i <= vertexCount; i++){
//         if(!visited[i]){
//             topologicalSortHelper(i, visited, adjList, vertices);
//         }
//     }

//     vector<int> order(vertexCount);
//     int i = 0;
//     while(!vertices.empty()){
//         order[i++] = vertices.top();
//         vertices.pop();
//     }

//     return order;
// }


void unweighted_undirected(Graph<int, GraphType_UnweightedUndirected, GraphRepresentation_AdjacencyList>& g2){
    int a2 = g2.addVertex(1);
    int b2 = g2.addVertex(2);
    int c2 = g2.addVertex(3);
    int d2 = g2.addVertex(4);
    int e2 = g2.addVertex(5);

    g2.addEdge(a2, b2);
    g2.addEdge(b2, c2);
    g2.addEdge(b2, d2);
    g2.addEdge(c2, d2);
}

void weighted_undirected(Graph<char, GraphType_WeightedUndirected, GraphRepresentation_AdjacencyList, int>& g1){
    int a1 = g1.addVertex('a');
    int b1 = g1.addVertex('b');
    int c1 = g1.addVertex('c');
    int d1 = g1.addVertex('d');
    int e1 = g1.addVertex('e');

    g1.addEdge(a1, b1, 1);
    g1.addEdge(b1, c1, 1);
    g1.addEdge(b1, d1, 3);
    g1.addEdge(c1, d1, 1);
}

void unweighted_directed(Graph<int, GraphType_UnweightedDirected, GraphRepresentation_AdjacencyList>& g2){
    int a2 = g2.addVertex(1);
    int b2 = g2.addVertex(2);
    int c2 = g2.addVertex(3);
    int d2 = g2.addVertex(4);
    int e2 = g2.addVertex(5);

    g2.addEdge(a2, b2);
    g2.addEdge(b2, c2);
    g2.addEdge(b2, d2);
    g2.addEdge(c2, d2);
}

void weighted_directed(Graph<int, GraphType_WeightedDirected, GraphRepresentation_AdjacencyList, int>& g1){  
    int a1 = g1.addVertex(1);
    int b1 = g1.addVertex(2);
    int c1 = g1.addVertex(3);
    int d1 = g1.addVertex(4);
    int e1 = g1.addVertex(5);

    g1.addEdge(a1, b1, 1);
    g1.addEdge(b1, c1, 1);
    g1.addEdge(b1, d1, 1);
    g1.addEdge(c1, d1, 1);
}

int main(){
    cout << "HELLO WORLD" << endl;
    
    Graph<int, GraphType_UnweightedUndirected, GraphRepresentation_AdjacencyList> g1;
    Graph<char, GraphType_WeightedUndirected, GraphRepresentation_AdjacencyList, int> g2;
    Graph<int, GraphType_UnweightedDirected, GraphRepresentation_AdjacencyList> g3;
    Graph<int, GraphType_WeightedDirected, GraphRepresentation_AdjacencyList, int> g4;

    unweighted_undirected(g1);
    weighted_undirected(g2);
    unweighted_directed(g3);
    weighted_directed(g4);


    auto result = g3.allPairShortestPath();
    for(auto ele: result.second){
        cout << g3.vertex_value(ele) << endl;
    }

    return 0;
}
