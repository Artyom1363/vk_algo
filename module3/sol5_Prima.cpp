/*

Требуется отыскать самый выгодный маршрут между городами. 
Требования: время работы O((N+M)logN), где N-количество городов, M-известных дорог между ними.
Формат входных данных.
Первая строка содержит число N – количество городов.
Вторая строка содержит число M - количество дорог.
Каждая следующая строка содержит описание дороги (откуда, куда, время в пути).
Последняя строка содержит маршрут (откуда и куда нужно доехать).
Формат выходных данных.
Вывести длину самого выгодного маршрута.

*/

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <optional>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::optional;
using std::set;
using std::pair;

template <class T>
class MST {
public:
    MST(size_t grafSize) : grafSize(grafSize) {}

    void AddDirectedEdge(const T &from, const T& to, unsigned int weight);

    size_t CalcMST();

private:
    unordered_map <T, vector <pair <T, int>>> edges;
    unordered_map <T, optional <unsigned int>> dists;
    size_t grafSize;
};

template <class T>
void MST<T>::AddDirectedEdge(const T &from, const T& to, unsigned int weight) {
    edges[from].push_back({to, weight});
}

template <class T>
size_t MST<T>::CalcMST() {
    size_t calcDist = 0;
    set <pair<int, T>> tmpDists;
    tmpDists.insert({0, 1});
    unordered_set <int> buildMST;
    while (tmpDists.size()) {
        pair <int, T> minVertex = *(tmpDists.begin());
        calcDist += minVertex.first;
        buildMST.insert(minVertex.second);
        tmpDists.erase(tmpDists.begin());
        for (auto &edge : edges[minVertex.second]) {
            T newVertex = edge.first;
            optional <unsigned int> lastDistToV = dists[newVertex];
            int nowDistToV = edge.second;
            if (!buildMST.count(newVertex) && (!lastDistToV || nowDistToV < lastDistToV)) {
                if (lastDistToV) {
                    pair <int, T> lastEdge = {*lastDistToV, newVertex};
                    tmpDists.erase(tmpDists.find(lastEdge));
                }
                dists[newVertex] = nowDistToV;
                tmpDists.insert({nowDistToV, newVertex});
            }
        }
    }
    return calcDist;
}


int main() {
    int n;
    int m;
    int from;
    int to;
    cin >> n >> m;
    MST <unsigned int> mst(n);
    for (int i = 0, u, v, w; i < m; ++i) {
        cin >> u >> v >> w;
        mst.AddDirectedEdge(u, v, w);
        mst.AddDirectedEdge(v, u, w);
    }
    size_t dist = mst.CalcMST();
    cout << dist;
    return 0;
}
