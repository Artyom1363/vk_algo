#include <iostream>
#include <cassert>

using std::cin;
using std::cout;
using std::endl;

template <typename T>
class AvlTree
{
    struct Node
    {
        Node(const T &data) : data(data), height(1), left(nullptr), right(nullptr), count(1) {}
        T data;
        size_t height;
        unsigned int count;
        Node *left;
        Node *right;
    };
    
public:
    AvlTree() : root(nullptr) {}
    
    ~AvlTree() { destroyTree(root); }

    T GetKthStat(unsigned int k) {
        return getKthElem(k + 1, root)->data;
    }
    
    void Add(const T &data) {
        root = addInternal(root, data);
    }
    
    bool Has(const T &data) {
        Node *tmp = root;
        while (tmp)
        {
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
    
private:
    Node *root;

    unsigned int getCount(Node *node) {
        return node ? node->count : 0;
    }
    
    unsigned int getIndex(Node* node) {
        return node ? (getCount(node) - getCount(node->right)) : 0;
    }

    Node* getKthElem(unsigned int k, Node* node) {
        unsigned int index = getIndex(node);
        assert(node);
        if (index == k) {
            return node;
        }
        if (index < k) {
            return getKthElem(k - index, node->right);
        } else {
            return getKthElem(k, node->left);
        }
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
        if (node->data < data) {
            node->right = deleteInternal(node->right, data);
        }
        else if (node->data > data) {
            node->left = deleteInternal(node->left, data);
        }
        else {
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
        Node *min = findMin(right);
        min->right = removeMin(right);
        min->left = left;
        return doBalance(min);
    }

    Node* findMin(Node *node) {
        while (node->left)
            node = node->left;
        return node;
    }
    
    Node* removeMin(Node *node) {
        if (!node->left)
            return node->right;
        node->left = removeMin(node->left);
        return doBalance(node);
    }
    
    Node* addInternal(Node *node, const T &data) {
        if (!node)
            return new Node(data);
        if (node->data <= data) {
            node->right = addInternal(node->right, data);
        } else {
            node->left = addInternal(node->left, data);
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

int main(int argc, const char * argv[]) {
    AvlTree <int> avlTree;
    unsigned int n;
    cin >> n;
    for (int i = 0, elem, k; i < n; ++i) {
        cin >> elem >> k;
        if (elem > 0) {
            avlTree.Add(elem);
        } else if (elem < 0) {
            avlTree.Delete(-elem);
        }
        cout << avlTree.GetKthStat(k) << " ";
    }
    return 0;
}
