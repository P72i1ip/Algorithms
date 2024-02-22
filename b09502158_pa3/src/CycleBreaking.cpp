#include "CycleBreaking.h"

//=============== Cycle Breaking ===================

void CycleBreaking::working(){
    if(Directed){
        while(true){
            if(!g->isCyclic(deletedEdges)){
                break;
            }
        }
        //g->isCyclic(deletedEdges);
    }
    else{
        g->maximumSpanningTree(deletedEdges);
    }
    for(int i = 0; i < deletedEdges.size(); i++){
        deleteWeightSum += deletedEdges[i]->weight;
    }
}

void CycleBreaking::readFile(const char *fileName){
    ifstream ifs(fileName);

    int vertex_num = 0, edge_num = 0;
    char graph_type;

    ifs >> graph_type;
    if(graph_type == 'd')
        Directed = true;
    ifs >> vertex_num;
    ifs >> edge_num;

    g = new Graph(vertex_num, edge_num, Directed);

    for(int i = 0; i < edge_num; i++){
        int u, v, w;
        ifs >> u >> v >> w;
        g->addEdge(u, v, w);
    }
    ifs.close();
}

void CycleBreaking::writeFile(const char *fileName){
    ofstream ofs(fileName);

    if(deletedEdges.empty()){
        ofs << "0" << endl;
    }

    else{
        ofs << deleteWeightSum << endl;
        for(auto i = 0; i < deletedEdges.size(); i++){
            ofs << deletedEdges[i]->u << " " << deletedEdges[i]->v << " " << deletedEdges[i]->weight << endl;
        }
    }

    ofs.close();
    delete g;
}
//==================================================



//================ Graph ===================

Graph::Graph(int vertex_num, int edge_num, bool directed): n_vertex(vertex_num), n_edge(edge_num), g_directed(directed), minEdge(-1, -1, -1){
    if(g_directed){
        adjList = new vector<pair<int, int> >[n_vertex];
    }
}

Graph::~Graph(){
    if(g_directed){
        // for(int i = 0; i < n_vertex; i++){
        //     for(auto j = adjList[i].begin(); j != adjList[i].end(); j++){
        //         cout << "vertex i: " << i << " " << (*j).first << " " << (*j).second << endl;
        //     }
        // }
        delete[] adjList;
    }
}

void Graph::addEdge(int u, int v, int weight){
    edges.push_back(Edge(u, v, weight));
    if(g_directed){
        adjList[u].push_back(make_pair(v, weight));
    }
}

//Kruskal to find maximum spanning tree, edges not in MST are deleted
void Graph::maximumSpanningTree(vector<Edge*> & deletedEdges){
    DisjointSet ds(n_vertex);

    sort(edges.begin(), edges.end());

    //loop through edges in descending order
    for(int i = edges.size()-1; i >= 0; i--){
        int rootU = ds.find(edges[i].u);
        int rootV = ds.find(edges[i].v);

        /*debug
        cout << "u: " << edges[i].u << " v: " << edges[i].v << " w: " << edges[i].weight << endl; 
        cout << "root U: " << rootU << " root V: " << rootV << endl; 
        cout << endl;
        */

        if (rootU != rootV) {
            ds.unionSets(rootU, rootV);
        }
        else{
            deletedEdges.push_back(&edges[i]);
        }
    }
}

bool Graph::isCyclicUntil(int v, bool visited[], bool* recStack, vector<Edge*> & deletedEdges){
    if(visited[v] == false){
        visited[v] = true;
        recStack[v] = true;
    }

    for(auto i = adjList[v].begin(); i != adjList[v].end(); i++){

        // cout << v << " " << (*i).first << " " << (*i).second << endl;

        if(!visited[(*i).first] && isCyclicUntil((*i).first, visited, recStack, deletedEdges)){
            if(!stop){
                if((*i).second < minEdge.weight){
                    minEdge.u = v;
                    minEdge.v = (*i).first;
                    minEdge.weight = (*i).second;
                    // cout << "min edge update:" << endl;
                    // cout << minEdge.u << " " << minEdge.v << " " << minEdge.weight << endl;
                }
            }
            if(v == cycleBegin){
                deletedEdges.push_back(new Edge(minEdge.u, minEdge.v, minEdge.weight));
                //deletedEdges.push_back(&minEdge); //BUG!!!
                // cout << "push" << endl;
                cout << "push minEdge:" << endl;
                cout << minEdge.u << " " << minEdge.v << " " << minEdge.weight << endl;
                
                // Use erase to remove the pair from the adjacency list of vertex v
                auto it = std::find(adjList[minEdge.u].begin(), adjList[minEdge.u].end(), std::make_pair(minEdge.v, minEdge.weight));
                if (it != adjList[minEdge.u].end()) {
                    adjList[minEdge.u].erase(it);
                } 

                // cout << "now in vertex: " << v << endl;
                // // Display the updated adjacency list
                // std::cout << "Updated Adjacency List:\n";
                // for (int i = 0; i < n_vertex; ++i) {
                //     std::cout << "Vertex " << i << ": ";
                //     for (const auto &edge : adjList[i]) {
                //         std::cout << "(" << edge.first << ", " << edge.second << ") ";
                //     }
                //     std::cout << "\n";
                // }

                stop = true;
            }
            return true;
        }

        else if(recStack[(*i).first]){
            cycleBegin = (*i).first;
            minEdge = Edge(v, (*i).first, (*i).second);
            // cout << "minEdge:" << endl;
            // cout << minEdge.u << " " << minEdge.v << " " << endl;
            return true;
        }
    }

    recStack[v] = false;
    return false;
}

//https://www.geeksforgeeks.org/detect-cycle-in-a-graph/
bool Graph::isCyclic(vector<Edge*> & deletedEdges){
    bool* visited = new bool[n_vertex];
    bool* recStack = new bool[n_vertex];

    for(int i = 0; i < n_vertex; i++){
        visited[i] = false;
        recStack[i] = false;
    }

    // // Display the original adjacency list
    // std::cout << "Original Adjacency List:\n";
    // for (int i = 0; i < n_vertex; ++i) {
    //     std::cout << "Vertex " << i << ": ";
    //     for (const auto &edge : adjList[i]) {
    //         std::cout << "(" << edge.first << ", " << edge.second << ") ";
    //     }
    //     std::cout << "\n";
    // }

    for(int i = 0; i < n_vertex; i++){
        //cout << "i: " << i << endl;
        
        if(isCyclicUntil(i, visited, recStack, deletedEdges)){
            stop = false;
            cycleBegin = -1;
            return true;
        }
    }

    delete[] visited;
    delete[] recStack;
    return false;
}
//==========================================



//=============== Disjoint Set ===================

DisjointSet::DisjointSet(int size) : parent(size), rank(size, 0) {
    for (int i = 0; i < size; ++i) {
        parent[i] = i;
    }
}

int DisjointSet::find(int u) {
    if (u != parent[u]) {
        parent[u] = find(parent[u]);
    }
    return parent[u];
}

void  DisjointSet::unionSets(int u, int v) {
    int rootU = find(u);
    int rootV = find(v);

    if (rootU != rootV) {
        if (rank[rootU] > rank[rootV]) {
            parent[rootV] = rootU;
        } else if (rank[rootU] < rank[rootV]) {
            parent[rootU] = rootV;
        } else {
            parent[rootV] = rootU;
            rank[rootU]++;
        }
    }
}
//==================================================