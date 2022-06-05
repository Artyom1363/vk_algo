/*

Дано число N ≤ 104 и последовательность целых чисел из [-231..231] длиной N. 
Требуется построить бинарное дерево, заданное наивным порядком вставки. 
Т.е., при добавлении очередного числа K в дерево с корнем root, 
если root→Key ≤ K, то узел K добавляется в правое поддерево root; 
иначе в левое поддерево root. Выведите элементы в порядке post-order (снизу вверх).
Рекурсия запрещена.

*/

#include <iostream>
#include <vector>

using std::vector;
using std::cout;

template <class T, class Cmp>
class BinaryTree {
public:
    BinaryTree(const Cmp& _cmp): root(nullptr), cmp(_cmp) {}
    ~BinaryTree();

    void Add(const T& key);
    vector <T> Show();

private:
    struct Node {
        Node* left;
        Node* right;
        T key;
        Node(): left(nullptr), right(nullptr) {}
        Node(const T& _key) : key(_key), left(nullptr), right(nullptr) {}
    };
    Cmp cmp; 
    Node* root;
};

template <class T, class Cmp>
void BinaryTree<T, Cmp>::Add(const T& key) {
    if (root == nullptr) {
        Node* node = new Node(key);
        root = node;
        return;
    }
    std::vector <Node*> st;
    Node* node = root;
    while (node != nullptr) {
        if (cmp(key, node->key)) {
            if (node->left == nullptr) {
                Node* newNode = new Node(key);
                node->left = newNode;
                return;
            } else {
                node = node->left;
            }
        } else {
            if (node->right == nullptr) {
                Node* newNode = new Node(key);
                node->right = newNode;
                return;
            } else {
                node = node->right;
            }
        }
    }
    node->key = key;
}

template <class T, class Cmp>
vector <T> BinaryTree<T, Cmp>::Show() {
    vector <T> answer;
    struct SmartVertex {
        Node* node;
        bool marked;
        SmartVertex(Node* _node) : node(_node), marked(false) {}
    };
    vector <SmartVertex> st;
    SmartVertex start(root);
    st.push_back(start);
    while (st.size() > 0) {
        SmartVertex& smartVertex = st.back();
        if (smartVertex.node == nullptr) {
            st.pop_back();
            continue;
        }
        if (smartVertex.marked) {
            st.pop_back();
            answer.push_back(smartVertex.node->key);
            continue;
        } else {
            smartVertex.marked = true;
        }
        SmartVertex left(smartVertex.node->left);
        SmartVertex right(smartVertex.node->right);
        st.push_back(right);
        st.push_back(left);
    }
    return answer;
}

template <class T, class Cmp>
BinaryTree<T, Cmp>::~BinaryTree() {
    vector <Node*> st;
    st.push_back(root);
    while (st.size() > 0) {
        Node* node = st.back();
        st.pop_back();
        if (node == nullptr) continue;
        st.push_back(node->left);
        st.push_back(node->right);
        delete node;
    }
}

class Cmp {
public:
    bool operator()(const int l, const int r) {
        return l < r;
    }
};

int main() {
    Cmp cmp;
    BinaryTree <int, Cmp> bt(cmp);
    unsigned int n;
    std::cin >> n;
    for (int i = 0, key; i < n; ++i) {
        std::cin >> key;
        bt.Add(key);
    }
    vector <int> ans = bt.Show();
    for (auto vertex : ans) {
        cout << vertex << " ";
    }
    return 0;
}