#include <iostream>
#include <vector>
#include <algorithm>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

class DSU {
public:
    DSU(int count);
    int FindParent(int v);
    void Union(int v1, int v2);
private:
    vector <int> parent;
};

DSU::DSU(int count) : parent(count + 1, 0) {
    for (int i = 0; i <= count; ++i) {
        parent[i] = i;
    }
}

int DSU::FindParent(int v) {
    if (parent[v] == v) {
        return v;
    }
    parent[v] = FindParent(parent[v]);
    return parent[v];
}

void DSU::Union(int v1, int v2) {
    int p1 = FindParent(v1);
    int p2 = FindParent(v2);
    if (p1 == p2) return;
    parent[p2] = p1;
    return;
}


struct Edge {
    size_t weight;
    size_t from;
    size_t to;
};

class Cmp {
public:
    bool operator()(const Edge& l, const Edge& r) {
        return l.weight < r.weight;
    }
};

std::ostream& operator<<(std::ostream& out, Edge& edge) {
    out << edge.weight << " " << edge.from << " " << edge.to << endl;
    return out;
}



void showDsu(DSU& dsu, int size) {
    for (int i = 1; i <= size; ++i) {
        cout << dsu.FindParent(i) << " ";
    }
    cout << endl;
}

class MST {
public:
    MST(vector <Edge>& edges, size_t grafSize) : edges(edges), grafSize(grafSize) {}

    size_t CalcMST();

private:
    size_t grafSize;
    vector <Edge> edges;
};

size_t MST::CalcMST() {
    Cmp cmp;
    std::sort(edges.begin(), edges.end(), cmp);
    DSU dsu(grafSize);
    size_t cnt = 0;
    for (int i = 0; i < edges.size(); ++i) {
        size_t fromParent = dsu.FindParent(edges[i].from);
        size_t toParent = dsu.FindParent(edges[i].to);
        if (fromParent != toParent) {
            cnt += edges[i].weight;
            dsu.Union(edges[i].from, edges[i].to);
        }
    }
    return cnt;
}

int main() {
    int n, m;
    cin >> n >> m;
    vector <Edge> edges(m);
    for (int i = 0; i < m; ++i) {
        cin >> edges[i].from >> edges[i].to >> edges[i].weight;
    }
    MST mst(edges, n);
    cout << mst.CalcMST() << endl;
    return 0;
}