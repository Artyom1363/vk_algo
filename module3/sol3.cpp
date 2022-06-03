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
class Dijkstra {
public:
    Dijkstra() {}

    void AddDirectedEdge(const T &from, const T& to, unsigned int weight);

    void FindDists(const T& from);

    optional<unsigned int> GetDistTo(const T& to);

private:
    unordered_map <T, vector <pair <T, int>>> edges;
    unordered_map <T, optional <unsigned int>> dists;

};

template <class T>
void Dijkstra<T>::AddDirectedEdge(const T &from, const T& to, unsigned int weight) {
    edges[from].push_back({to, weight});
}

template <class T>
void Dijkstra<T>::FindDists(const T& from) {
    set <pair<int, T>> tmpDists;
    tmpDists.insert({0, from});
    unordered_set <int> used;
    while (tmpDists.size()) {
        pair <int, T> minVertex = *(tmpDists.begin());
        used.insert(minVertex.second);
        tmpDists.erase(tmpDists.begin());
        for (auto &edge : edges[minVertex.second]) {
            T newVertex = edge.first;
            optional <unsigned int> lastDistToV = dists[newVertex];
            int nowDistToV = minVertex.first + edge.second;
            if (!used.count(newVertex) && (!lastDistToV || nowDistToV < lastDistToV)) {
                if (lastDistToV) {
                    pair <int, T> lastEdge = {*lastDistToV, newVertex};
                    tmpDists.erase(tmpDists.find(lastEdge));
                }
                dists[newVertex] = nowDistToV;
                tmpDists.insert({nowDistToV, newVertex});
            }
        }
    }
}

template <class T>
optional<unsigned int> Dijkstra<T>::GetDistTo(const T& to) {
    return dists[to];
}

int main() {
    Dijkstra <unsigned int> dijkstra;
    int n;
    int m;
    int from;
    int to;
    cin >> n >> m;
    for (int i = 0, u, v, w; i < m; ++i) {
        cin >> u >> v >> w;
        dijkstra.AddEdge(u, v, w);
        dijkstra.AddEdge(v, u, w);
    }
    cin >> from >> to;
    dijkstra.FindDists(from);
    optional <int> dist = dijkstra.GetDistTo(to);
    if (dist) {
        cout << *dist << endl;
    }
    return 0;
}
/*
6
9
0 3 1
0 4 2
1 2 7
1 3 2
1 4 3
1 5 3
2 5 3
3 4 4
3 5 6
0 2

3
3
0 1 1
1 1 2
1 2 6
2 0

*/