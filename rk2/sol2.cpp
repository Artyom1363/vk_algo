#include <iostream>
#include <vector>
#include <set>
#include <cassert>
#include <sstream>

using std::cin;
using std::cout;
using std::endl;

template <class T, class Cmp>
class BinaryTree {
public:
    BinaryTree(const Cmp& _cmp): root(nullptr), cmp(_cmp) {}
    ~BinaryTree();

    void Add(const T& key);
    void Show();
    unsigned int FindMinDist();

private:
    struct Node {
        Node* left;
        Node* right;
        T key;
        Node(): left(nullptr), right(nullptr) {}
        Node(const T& _key) : key(_key), left(nullptr), right(nullptr) {}
        bool IsSheet() {
            return (!left && !right);
        }
    };
    Cmp cmp; // <
    Node* root;
    // void AddNode(Node*& node, const T& key);
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
BinaryTree<T, Cmp>::~BinaryTree() {
    std::vector <Node*> st;
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

template <class T, class Cmp>
unsigned int BinaryTree<T, Cmp>::FindMinDist() {
    struct NodeWithDist {
        Node* node;
        unsigned int dist;
        NodeWithDist() : dist(0) {}
        NodeWithDist(Node* node, unsigned int dist) : node(node), dist(dist) {}
    };
    std::vector <NodeWithDist> st;
    unsigned int minDist = 1e9;
    if (root == nullptr) return 0;
    NodeWithDist nodeWithDist(root, 1);
    st.push_back(nodeWithDist);
    while (st.size() > 0) {
        NodeWithDist nodeWithDist = st.back();
        st.pop_back();
        Node* node = nodeWithDist.node;
        if (node == nullptr) continue;
        st.push_back(NodeWithDist(node->left, (nodeWithDist.dist) + 1));
        st.push_back(NodeWithDist(node->right, (nodeWithDist.dist) + 1));
        if (node->IsSheet()) {
            minDist = std::min(minDist, nodeWithDist.dist);
        }
    }
    return minDist;
}

class Cmp {
public:
    bool operator()(const int l, const int r) {
        return l < r;
    }
};

void run(std::istream &input, std::ostream &output) {
    Cmp cmp;
    BinaryTree <int, Cmp> bt(cmp);
    int key;
    while (input >> key) {
        bt.Add(key);
    }
    output << bt.FindMinDist();
}



void test() {
    {   
        std::stringstream input;
        std::stringstream output;
        input << "2 1 3";
        run(input, output);
        assert(output.str() == "2"); 
    }
    {   
        std::stringstream input;
        std::stringstream output;
        input << "3 2 1 4";
        run(input, output);
        assert(output.str() == "2"); 
    }
}


int main() {
    // run(cin, cout);
    test();
    return 0;
}