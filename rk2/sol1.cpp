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
    bool AllElemsEqual();

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
bool BinaryTree<T, Cmp>::AllElemsEqual() {
    std::vector <Node*> st;
    std::set <T> setOfValues;
    st.push_back(root);
    while (st.size() > 0) {
        Node* node = st.back();
        st.pop_back();
        if (node == nullptr) continue;
        setOfValues.insert(node->key);
        st.push_back(node->left);
        st.push_back(node->right);
    }
    return setOfValues.size() == 1;
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
    output << (bt.AllElemsEqual() ? 1 : 0);

}

void test() {
    {   
        std::stringstream input;
        std::stringstream output;
        input << "2 3";
        run(input, output);
        assert(output.str() == "0"); 
    }
    {   
        std::stringstream input;
        std::stringstream output;
        input << "1 1 1";
        run(input, output);
        assert(output.str() == "1"); 
    }
}

int main() {
    // run(cin, cout);
    test();
    return 0;
}