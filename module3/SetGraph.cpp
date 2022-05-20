#include "SetGraph.h"

using std::vector;
using std::unordered_set;

SetGraph::SetGraph(int counter) {
    edges.resize(counter);
}

SetGraph::SetGraph(const IGraph& graph) {
    int size = graph.VerticesCount();
    for (int i = 0; i < size; ++i) {
        vector <int> vertexes = graph.GetNextVertices(i);
        for (auto vertexTo : vertexes) {
            edges[i].insert(vertexTo);
        }
    }
}

void SetGraph::AddEdge(int from, int to) {
    edges[from].insert(to);
}

int SetGraph::VerticesCount() const {
    return edges.size();
}

vector<int> SetGraph::GetNextVertices(int vertex) const {
    vector <int> edgesFromVertex;
    for (auto vertexTo : edges[vertex]) {
        edgesFromVertex.push_back(vertexTo);
    }
    return edgesFromVertex;
}

vector<int> SetGraph::GetPrevVertices(int vertex) const {
    vector <int> edgesToVertex;
    for (int i = 0; i < edges.size(); ++i) {
        for (auto vertexTo : edges[i]) {
            if (vertexTo == vertex) {
                edgesToVertex.push_back(i);
            }
        }
    }
    return edgesToVertex;
}