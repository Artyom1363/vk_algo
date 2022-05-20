#include "MatrixGraph.h"

using std::vector;

MatrixGraph::MatrixGraph(int counter) {
    matrix.resize(counter);
    for (int i = 0; i < counter; ++i) {
        matrix[i].resize(counter, 0);
    }
}

MatrixGraph::MatrixGraph(const IGraph& graph) {
    int size = graph.VerticesCount();
    for (int i = 0; i < size; ++i) {
        vector <int> edges = graph.GetNextVertices(i);
        for (auto vertexTo : edges) {
            matrix[i][vertexTo] = 1;
        }
    }
}

void MatrixGraph::AddEdge(int from, int to) {
    matrix[from][to] = 1;
}

int MatrixGraph::VerticesCount() const {
    return matrix.size();
}

vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    vector <int> edges;
    for (int i = 0; i < matrix.size(); ++i) {
        if (matrix[vertex][i]) {
            edges.push_back(i);
        }
    }
    return edges;
}

vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    vector <int> edges;
    for (int i = 0; i < matrix.size(); ++i) {
        if (matrix[i][vertex]) {
            edges.push_back(i);
        }
    }
    return edges;
}