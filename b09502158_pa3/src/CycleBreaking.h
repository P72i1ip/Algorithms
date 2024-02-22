#ifndef _CYCLEBREAKING_H_
#define _CYCLEBREAKING_H_

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <utility>
using namespace std;

struct Edge{
    int u, v, weight;

    Edge(int u, int v, int weight) : u(u), v(v), weight(weight) {}

    // Sort edges in descending order based on their weights
    // for Kruskal to find maximum spanning tree
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

class Graph{
public:
    Graph(int, int, bool);
    ~Graph();
    void addEdge(int u, int v, int weight);
    void maximumSpanningTree(vector<Edge*> &);
    bool isCyclic(vector<Edge*> &);
    bool isCyclicUntil(int, bool[], bool*, vector<Edge*> &);

private:
    int n_vertex;
    int n_edge;
    bool g_directed;
    vector<Edge> edges;

    //for directed only
    vector<pair<int, int> > *adjList; //adjacent list: adjList[i] = (j, weight)
    Edge minEdge;
    bool stop;
    int cycleBegin;
    // int iter = 0; //iterator for cycle detect
};


class CycleBreaking{
public:
    //CycleBreaking();
    //~CycleBreaking();
    void readFile(const char*);
    void writeFile(const char*);
    void working();

private:
    bool Directed = false;
    Graph *g;
    vector<Edge*> deletedEdges;
    int deleteWeightSum = 0;
};

class DisjointSet {
public:
    DisjointSet(int size);
    //~DisjointSet();
    int find(int u);
    void unionSets(int u, int v);

private:
    vector<int> parent;
    vector<int> rank;
};

#endif // _CYCLEBREAKING_H_