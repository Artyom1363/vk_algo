/*

Дан невзвешенный неориентированный граф. 
В графе может быть несколько кратчайших путей между какими-то вершинами. 
Найдите количество различных кратчайших путей между заданными вершинами. 

*/

#include <iostream>
#include <vector>
#include <queue>

using std::vector;
using std::cin;
using std::cout;
using std::endl;
using std::queue;

int CountShortestRoutes(int from, int to, vector <vector <int>>& graf) {

    queue <int> bfsQueue;
    vector <int> vertexLevel(graf.size());
    vector <int> counter(graf.size());
    counter[from] = 1;
    bfsQueue.push(from);
    bfsQueue.push(-1);
    int level = 1;
    while (bfsQueue.size() > 1) {
        int currentVertex = bfsQueue.front();
        bfsQueue.pop();
        if (currentVertex == -1) {
            bfsQueue.push(currentVertex);
            ++level;
            continue;
        }
        vertexLevel[currentVertex] = level;
        for (auto vertexTo : graf[currentVertex]) {
            if (vertexLevel[vertexTo] == 0 || vertexLevel[vertexTo] == level + 1) {
                if (vertexLevel[vertexTo] == 0) bfsQueue.push(vertexTo);
                counter[vertexTo] += counter[currentVertex];
                vertexLevel[vertexTo] = level + 1;
            }
        }
    }
    return counter[to];
}



int main() {
    int n;
    int m;
    int from;
    int to;
    cin >> n >> m;
    vector <vector <int>> graf(n);
    for (int i = 0, u, v; i < m; ++i) {
        cin >> u >> v;
        graf[u].push_back(v);
        graf[v].push_back(u);
    }
    cin >> from >> to;
    cout << CountShortestRoutes(from, to, graf);
    return 0;
}