#include <iostream>
#include <array>
#include <cassert>
#include <queue>
#include <vector>
#include <algorithm>


using std::cin;
using std::cout;
using std::endl;
using std::queue;
using std::vector;


size_t NumberOfConnectComponents(vector <vector<int>> &graf) {
    vector <int> used(graf.size());
    size_t cnt = 0;
    for (int i = 0; i < graf.size(); ++i) {
        if (!used[i]) {
            ++cnt;
            used[i] = 1;
            queue <int> q;
            q.push(i);
            while (q.size()) {
                int cur = q.front();
                q.pop();
                for (auto v : graf[cur]) {
                    if (!used[v]) {
                        used[v] = 1;
                        q.push(v);
                    }
                }
            }
        }
    }
    return cnt;
}

int main() {
    int n, m;
    cin >> n >> m;
    vector <vector <int>> graf(n);
    for (int i = 0, u, v; i < m; ++i) {
        cin >> u >> v;
        graf[u].push_back(v);
        graf[v].push_back(u);
    }
    cout << NumberOfConnectComponents(graf);
    return 0;
}