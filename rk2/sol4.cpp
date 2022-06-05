#include <iostream>
#include <cassert>
#include <optional>
#include <cassert>
#include <sstream>

using std::cin;
using std::cout;
using std::endl;
using std::string;

template<class T>
class IsLess {
public:
	bool operator()( const T& l, const T& r ) { 
        return l < r;
    }
};


template <class T, class Compare = IsLess<T> >
class AvlTree {
public:
    AvlTree(Compare cmp) : cmp(cmp), root(nullptr) {}
    
    ~AvlTree() { destroyTree(root); }
    
    void Add(const T &data) {
        if (!Has(data)) {
            root = addInternal(root, data);
        }
    }
    
    bool Has(const T &data) {
        Node *tmp = root;
        while (tmp) {
            if (tmp->data == data) {
                return true;
            } else if (tmp->data < data) {
                tmp = tmp->right;
            } else {
                tmp = tmp->left;
            }
        }
        return false;
    }
    
    void Delete(const T &data) {
        root = deleteInternal(root, data);
    }

    std::optional <T> Next(const T &data) {
        return nextNode(data, root);
    }
    std::optional <T> Prev(const T &data) {
        return prevNode(data, root);
    }


private:
    struct Node
    {
        Node(const T &data) : data(data), height(1), left(nullptr), right(nullptr), count(1) {}
        T data;
        size_t height;
        unsigned int count;
        Node *left;
        Node *right;
    };

    Node *root;
    Compare cmp;

    std::optional <T> nextNode(const T &data, Node* node) {
        if (node == nullptr) {
            return std::nullopt;
        }
        std::optional <T> value;
        if (cmp(data, node->data)) {
            value = nextNode(data, node->left);
        } else {
            value = nextNode(data, node->right);
        }
        if (!value && cmp(data, node->data)) {
            value = {node->data};
        }
        return value;
    }

    std::optional <T> prevNode(const T &data, Node* node) {
        if (node == nullptr) {
            return std::nullopt;
        }
        std::optional <T> value;
        if (cmp(node->data, data)) {
            value = prevNode(data, node->right);
        } else {
            value = prevNode(data, node->left);
        }
        if (!value && cmp(node->data, data)) {
            value = {node->data};
        }
        return value;
    }

    unsigned int getCount(Node *node) {
        return node ? node->count : 0;
    }

    void destroyTree(Node *node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
    
    Node* deleteInternal(Node *node, const T &data) {
        if (!node)
            return nullptr;
        if (cmp(node->data, data)) {
            node->right = deleteInternal(node->right, data);
        } else if (cmp(data, node->data)) {
            node->left = deleteInternal(node->left, data);
        } else {
            Node *left = node->left;
            Node *right = node->right;
            
            delete node;
            
            if (!right)
                return left;
            
            return setMinInsteadOf(left, right);
        }
        return doBalance(node);
    }
    

    Node* setMinInsteadOf(Node* left, Node* right) {
        int delta = getHeight(right) - getHeight(left);
        std::pair <Node*, Node*> nodes;
        Node* min;
        
        if (delta == -1) {
            nodes = findAndRemoveMax(left);
            min = nodes.second;
            min->right = right;
            min->left = nodes.first;
        } else {
            nodes = findAndRemoveMin(right);
            min = nodes.second;
            min->right = nodes.first;
            min->left = left;
        }
        
        return doBalance(min);
    }

    std::pair <Node*, Node*> findAndRemoveMin(Node* node) {
        std::pair <Node*, Node*> nodes;
        if (!node->left) {
            nodes.first = node->right;
            nodes.second = node;
            return nodes;
        }
        nodes = findAndRemoveMin(node->left);
        node->left = nodes.first;
        node = doBalance(node);
        nodes.first = node;
        return nodes;
    }

    std::pair <Node*, Node*> findAndRemoveMax(Node* node) {
        std::pair <Node*, Node*> nodes;
        if (!node->right) {
            nodes.first = node->left;
            nodes.second = node;
            return nodes;
        }
        nodes = findAndRemoveMax(node->right);
        node->right = nodes.first;
        node = doBalance(node);
        nodes.first = node;
        return nodes;
    }

    
    Node* addInternal(Node *node, const T &data) {
        if (!node)
            return new Node(data);
        if (cmp(data, node->data)) {
            node->left = addInternal(node->left, data);
        } else {
            node->right = addInternal(node->right, data);
        }
        return doBalance(node);
    }
    
    size_t getHeight(Node *node) {
        return node ? node->height : 0;
    }
    
    void updNodeInfo(Node *node) {
        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
        node->count = getCount(node->left) + getCount(node->right) + 1;
    }
    
    Node* rotateLeft(Node *node) {
        Node *tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;
        updNodeInfo(node);
        updNodeInfo(tmp);
        return tmp;
    }
    
    Node* rotateRight(Node *node) {
        Node *tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;
        updNodeInfo(node);
        updNodeInfo(tmp);
        return tmp;
    }
    
    int getBalance(Node *node) {
        return getHeight(node->right) - getHeight(node->left);
    }
    
    Node* doBalance(Node *node) {
        updNodeInfo(node);
        
        switch (getBalance(node)) {
            case 2: {
                if (getBalance(node->right) < 0)
                    node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
            case -2: {
                if (getBalance(node->left) > 0)
                    node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
            default:
                return node;
        }
    }
    
};


void run(std::istream &input, std::ostream &output) {
    IsLess <int> isLess;
    AvlTree <int> avlTree(isLess);
    string s;
    unsigned int num;
    while (input >> s >> num) {
        if (s == "insert") {
            avlTree.Add(num);
        } else if (s == "delete") {
            avlTree.Delete(num);
        } else if (s == "exists") {
            cout << (avlTree.Has(num) ? "true" : "false");
            cout << endl;
        } else if (s == "next") {
            std::optional <int>  value;
            value = avlTree.Next(num);
            if (value) {
                cout << *value;
            } else {
                cout << "none";
            }
            cout << endl;
        } else if (s == "prev") {
            std::optional <int>  value = avlTree.Prev(num);
            if (value) {
                cout << *value;
            } else {
                cout << "none";
            }
            cout << endl;
        }
    }
}

void test() {
    {   
        std::stringstream input;
        std::stringstream output;
        input << "insert 2 insert 5 insert 3 exists 2 exists 4 next 4 prev 4 delete 5 next 4 prev 4";
        run(input, output);
        cout << output.str() << endl;
        assert(output.str() == "true\nfalse\n5\n3\nnone\n3"); 
    }
}

int main(int argc, const char * argv[]) {
    run(cin, cout);
    // test();
    return 0;
}