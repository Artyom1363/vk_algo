#pragma once
#include <vector>
#include <utility>


#include "IGraph.h"

using std::vector;




struct ArcGraph : public IGraph {
    virtual ~ArcGraph() {}

    ArcGraph(int counter);

    ArcGraph(const IGraph& graph);

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    vector <std::pair<int, int>> allEdges;
    int vertexQuan;
};