#include <iostream>
#include <array>
#include <cassert>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <vector>
#include <cstring>
#include <algorithm>
#include <set>


using std::cin;
using std::cout;
using std::unordered_map;
using std::unordered_set;
using std::endl;
using std::set;
using std::pair;
using std::string;
using std::vector;


template <class T>
class Dijkstra {
public:
    Dijkstra(const size_t Mod, const size_t a, const size_t b) : Mod(Mod), a(a), b(b) {
        for (int i = 0; i < Mod; ++i) {
            dists[i] = INF;
        }
    }

    long long FindDist(const T& from, const T& to);

private:
    unordered_map <T, long long> dists;
    long long INF = 1e9;
    size_t Mod;
    size_t a;
    size_t b;

};


template <class T>
long long Dijkstra<T>::FindDist(const T& from, const T& to) {
    set <pair<long long, long long>> tmpDists;
    tmpDists.insert({0, from});
    vector <long long> used(Mod);
    while (tmpDists.size()) {
        pair <long long, T> minVertex = *(tmpDists.begin());
        if (minVertex.second == to) {
            return minVertex.first;
        }
        used[minVertex.second] = 1;
        tmpDists.erase(tmpDists.begin());
        vector <pair<long long, long long>> edges;
        edges.push_back({(minVertex.second + 1) % Mod, a});
        edges.push_back({(minVertex.second * minVertex.second + 1) % Mod, b});
        for (auto &edge : edges) {
            T newVertex = edge.first;
            long long lastDistToV = dists[newVertex];
            long long nowDistToV = minVertex.first + edge.second;
            if (!used[newVertex] && (nowDistToV < lastDistToV)) {
                if (lastDistToV != INF) {
                    pair <long long, T> lastEdge = {lastDistToV, newVertex};
                    tmpDists.erase(tmpDists.find(lastEdge));
                }
                dists[newVertex] = nowDistToV;
                tmpDists.insert({nowDistToV, newVertex});
            }
        }
    }
    return 0;
}


int main() {
    int a, b, m, x, y;
    cin >> a >> b >> m >> x >> y;
    Dijkstra <long long> dijkstra(m, a, b);
    cout << dijkstra.FindDist(x, y);
    return 0;
}