#pragma once
#include <vector>
#include <unordered_set>

#include "IGraph.h"

using std::vector;
using std::unordered_set;



struct SetGraph : public IGraph {
    virtual ~SetGraph() {}

    SetGraph(int counter);

    SetGraph(const IGraph& graph);

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;
private:
    vector <unordered_set<int>> edges;
};