#pragma once
#include <vector>

#include "IGraph.h"

using std::vector;


struct MatrixGraph : public IGraph {
    virtual ~MatrixGraph() {}

    MatrixGraph(int counter);

    MatrixGraph(const IGraph& graph);

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;
private:
    vector <vector<int>> matrix;
};