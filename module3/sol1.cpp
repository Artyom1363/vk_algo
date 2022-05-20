#include <iostream>
#include <vector>
#include <set>
#include <cassert>

#include "MatrixGraph.h"
#include "SetGraph.h"
#include "ArcGraph.h"

using std::set;
using std::vector;
using std::cout;
using std::endl;


void CopyVectorToSet(vector <int>& elemsVector, set <int>& elemsSet) {
    for (auto elem : elemsVector) {
        elemsSet.insert(elem);
    }
}

void ShowVector(vector <int>& v) {
    cout << "Vector: " << endl;
    for (auto elem : v) {
        cout << elem << " ";
    }
    cout << endl;
}

void testGraphs(IGraph& g1, IGraph& g2) {
    assert(g1.VerticesCount() == g2.VerticesCount());
    int cnt = g1.VerticesCount();
    for (int i = 0; i < cnt; ++i) {
        set <int> g1Set;
        set <int> g2Set;
        vector <int> g1Vector = g1.GetNextVertices(i);
        vector <int> g2Vector = g2.GetNextVertices(i);
        // ShowVector(matrixVector);
        // ShowVector(setVector);
        CopyVectorToSet(g1Vector, g1Set);
        CopyVectorToSet(g2Vector, g2Set);
        assert(g1Set == g2Set);


        g1Vector = g1.GetPrevVertices(i);
        g2Vector = g2.GetPrevVertices(i);
        g1Set.clear();
        g2Set.clear();
        // ShowVector(matrixVector);
        // ShowVector(setVector);

        CopyVectorToSet(g1Vector, g1Set);
        CopyVectorToSet(g2Vector, g2Set);
        assert(g1Set == g2Set);
    }
}

void test() {
    int cnt = 5;
    MatrixGraph matrixGraph(cnt);
    SetGraph setGraph(cnt);
    ArcGraph arcGraph(cnt);

    matrixGraph.AddEdge(1, 3);
    setGraph.AddEdge(1, 3);
    arcGraph.AddEdge(1, 3);

    matrixGraph.AddEdge(1, 2);
    setGraph.AddEdge(1, 2);
    arcGraph.AddEdge(1, 2);

    matrixGraph.AddEdge(1, 4);
    setGraph.AddEdge(1, 4);
    arcGraph.AddEdge(1, 4);
    
    matrixGraph.AddEdge(1, 0);
    setGraph.AddEdge(1, 0);
    arcGraph.AddEdge(1, 0);

    matrixGraph.AddEdge(2, 0);
    setGraph.AddEdge(2, 0);
    arcGraph.AddEdge(2, 0);
    
    matrixGraph.AddEdge(2, 3);
    setGraph.AddEdge(2, 3);
    arcGraph.AddEdge(2, 3);

    
    testGraphs(matrixGraph, setGraph);
    testGraphs(matrixGraph, arcGraph);

    MatrixGraph testMatrix(matrixGraph);
    SetGraph testSet(setGraph);
    ArcGraph testArc(arcGraph);

    testGraphs(matrixGraph, testMatrix);
    testGraphs(testSet, setGraph);
    testGraphs(testArc, arcGraph);
}


int main() {
    test();
    return 0;
}