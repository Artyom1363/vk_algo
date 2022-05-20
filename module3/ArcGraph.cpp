#include "ArcGraph.h"

using std::vector;
using std::pair;

ArcGraph::ArcGraph(int counter) : vertexQuan(counter) {

}

ArcGraph::ArcGraph(const IGraph& graph) {
    vertexQuan = graph.VerticesCount();
    for (int i = 0; i < vertexQuan; ++i) {
        vector <int> vertexes = graph.GetNextVertices(i);
        for (auto vertexTo : vertexes) {
            allEdges.push_back({i, vertexTo});
        }
    }
}

void ArcGraph::AddEdge(int from, int to) {
    allEdges.push_back({from, to});
}

int ArcGraph::VerticesCount() const {
    return vertexQuan;
}

vector<int> ArcGraph::GetNextVertices(int vertex) const {
    vector <int> edgesFromVertex;
    for (auto edge : allEdges) {
        if (edge.first == vertex) {
            edgesFromVertex.push_back(edge.second);
        }
    }
    return edgesFromVertex;
}

vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    vector <int> edgesToVertex;
    for (auto edge : allEdges) {
        if (edge.second == vertex) {
            edgesToVertex.push_back(edge.first);
        }
    }
    return edgesToVertex;
}