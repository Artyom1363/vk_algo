#include <iostream>
#include <vector>
#include <cassert>
#include <sstream>
#include <iterator>
#include <numeric>

using std::string;
using std::cin;
using std::cout;
using std::endl;

struct Node {
    std::string product_name;
    int color;
    int size;
    
    Node(string& product_name, int color, int size) : product_name(product_name), color(color), size(size) {}
    Node() : color(0), size(0) {}
};

bool operator==(const Node& l, const Node& r) {
    return (l.product_name == r.product_name && l.color == r.color && l.size == r.size);
}

class NodeHasher {
public:
    unsigned int operator()( const Node& node ) const {
        string str = node.product_name;
        unsigned int hash = 0;
        const unsigned int mult = 137;
        for (auto sym : str) {
            hash = hash * mult + sym;
        }
        return hash;
    }
};

template <class T, class H>
class UnorderedSet {
public:
    UnorderedSet(int initSize, const H& hasher);
    ~UnorderedSet();

    bool Has(const T& key) const;
    bool Add(const T& key);
    bool Delete(const T& key);

    void printSizes() {
        std::cout << "keysCount: " << keysCount << std::endl;
        std::cout << "table.size: " << table.size() << std::endl << std::endl;
    }
    void showTable() {
        std::cout << "START SHOW TABLE: \n";
        printSizes();
        for (auto hashTableNode: table) {
            std::cout << hashTableNode.key << " " << hashTableNode.hash << std::endl;
        }
        std::cout << "END OF SHOW TABLE! \n";
    }

private:
    enum class States {notUsed, marked, used};
    struct HashTableNode {
        unsigned int hash;
        T key;
        States mark;
        HashTableNode(): hash(0), mark(States::notUsed) {}
        HashTableNode(const T& key): key(key), hash(0), mark(States::used) {}
        HashTableNode(const T& key, const unsigned int fullHash): key(key), hash(fullHash), mark(States::used) {}
        void AddValue(const T& gotKey, const unsigned int fullHash) {
            key = gotKey;
            hash = fullHash;
            mark = States::used;
        }
        States getState() const {
            return mark;
        }
    };
    unsigned int keysCount;
    std::vector <HashTableNode> table;
    H hasher;
    void growTable();
    unsigned int getNextProb(const unsigned int i, const unsigned int prevValue) const;
};

template <class T, class H>
UnorderedSet<T, H>::UnorderedSet(int initSize, const H& hasher):  
    hasher(hasher),
    keysCount(0),
    table(initSize) {}

template <class T, class H>
UnorderedSet<T, H>::~UnorderedSet() {}

template <class T, class H>
bool UnorderedSet<T, H>::Has(const T& key) const {
    unsigned int hash = hasher(key) % table.size();
    // bool found = false;
    int iterations = 0;
    while (table[hash].getState() != States::notUsed) {
        // std::cout << "Has";
        ++iterations;
        if (iterations > table.size()) {
            return false;
        }
        if (table[hash].getState() == States::used && table[hash].key == key) {
            return true;
        }
        hash = getNextProb(hash, hash);
        // std::cout << "hash in Has: " << hash << std::endl;
    }
    return false;

}

template <class T, class H>
bool UnorderedSet<T, H>::Add(const T& key) {
    if (Has(key)) {
        return false;
    }
    if (keysCount * 4 >= table.size() * 3) {
        growTable();
    }
    unsigned int fullHash = hasher(key);
    unsigned int hash = fullHash % table.size();
    
    while (table[hash].getState() == States::used) {
        hash = getNextProb(hash, hash);
    }
    table[hash].AddValue(key, fullHash);
    ++keysCount;
    return true;
}

template <class T, class H>
bool UnorderedSet<T, H>::Delete(const T& key) {
    unsigned int iterations = 0;
    unsigned int hash = hasher(key) % table.size();

    while (table[hash].getState() != States::notUsed) {
        if (iterations > table.size()) {
            return false;
        }
        ++iterations;
        if (table[hash].getState() == States::used && table[hash].key == key) {
            table[hash].hash = 0;
            table[hash].mark = States::marked;
            --keysCount;
            return true;
        }
        // std::cout << "yes\n";
        hash = getNextProb(hash, hash);
    }
    return false;
}


template <class T, class H>
void UnorderedSet<T, H>::growTable() {
    unsigned int newTableSize = 2 * table.size();
    // showTable();
    std::vector <HashTableNode> oldTable = std::move(table);
    std::vector <HashTableNode> newTable(newTableSize);
    table = std::move(newTable);

    for (auto hashTableNode : oldTable) {
        if (hashTableNode.mark != States::used) {
            continue;
        }
        unsigned int hash = hashTableNode.hash % newTableSize;
        while (table[hash].getState() == States::used) {
            hash = getNextProb(hash, hash);
        }
        table[hash] = hashTableNode;
    }
}

template <class T, class H>
unsigned int UnorderedSet<T, H>::getNextProb(const unsigned int i, 
                                          const unsigned int prevValue) const {
    return (prevValue + 1) % table.size();
}





void run(std::istream &input, std::ostream &output) {
    NodeHasher hasher;
    UnorderedSet <Node, NodeHasher> unorderedSet(8, hasher);
    char operation;
	string product_name;
    int color, size;
    while( input >> operation >> product_name >> color >> size ) {
        Node node(product_name, color, size);
		if( operation == '+' )
			output << (unorderedSet.Add( node ) ? "OK" : "FAIL") << std::endl;
		if( operation == '?' )
			output << (unorderedSet.Has( node ) ? "OK" : "FAIL") << std::endl;
		if( operation == '-' )
			output << (unorderedSet.Delete( node ) ? "OK" : "FAIL") << std::endl;
	}

}



int main() {
    // test();
    run(cin, cout);
    return 0;
}
