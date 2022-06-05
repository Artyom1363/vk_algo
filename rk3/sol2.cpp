#include <iostream>
#include <array>
#include <cassert>
#include <queue>
#include <vector>
#include <algorithm>
#include <set>


using std::cin;
using std::cout;
using std::endl;
using std::queue;
using std::vector;
using std::set;

int main() {
    int n, m, k;
    cin >> n >> m >> k;
    vector < set<int> > graf(n);
    for (int i = 0, u, v; i < m; ++i) {
        cin >> u >> v;
        graf[u].insert(v);
        graf[v].insert(u);
    }
    set <int> vertexes;
    int prev;
    cin >> prev;
    bool fl = true;
    vertexes.insert(prev);
    for (int i = 1, u; i < k; ++i) {
        cin >> u;
        if (!graf[prev].count(u)) {
            fl = false;
        }
        prev = u;
        vertexes.insert(u);
    }
    cout << ((n == k && k == vertexes.size() && fl) ? 1 : 0) << endl;
    return 0;
}