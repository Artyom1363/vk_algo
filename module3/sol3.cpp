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

class Dijkstra {
public:
    Dijkstra() {}

    void AddEdge(int from, int to, int weight = 1) {
        edges[from].push_back({to, weight});
    }

    void FindDists(int from) {
        set <pair<int, int>> tmpDists;
        tmpDists.insert({0, from});
        unordered_set <int> used;
        while (tmpDists.size()) {
            pair <int, int> minVertex = *(tmpDists.begin());
            used.insert(minVertex.second);
            tmpDists.erase(tmpDists.begin());
            for (auto &edge : edges[minVertex.second]) {
                int newVertex = edge.first;
                optional <int> lastDistToV = dists[newVertex];
                int nowDistToV = minVertex.first + edge.second;
                if (!used.count(newVertex) && (!lastDistToV || nowDistToV < lastDistToV)) {
                    if (lastDistToV) {
                        pair <int, int> lastEdge = {*lastDistToV, newVertex};
                        tmpDists.erase(tmpDists.find(lastEdge));
                    }
                    dists[newVertex] = nowDistToV;
                    tmpDists.insert({nowDistToV, newVertex});
                }
            }
        }
    }
    optional<int> GetMinDistTo(int to) {
        return dists[to];
    }
private:
    unordered_map <int, vector <pair <int, int>>> edges;
    unordered_map <int, optional <int>> dists;

};

int main() {
    Dijkstra dijkstra;
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
    optional <int> dist = dijkstra.GetMinDistTo(to);
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