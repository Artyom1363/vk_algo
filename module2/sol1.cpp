/*
Реализуйте структуру данных типа “множество строк” на основе динамической 
хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят 
из строчных латинских букв.
Хеш-функция строки должна быть реализована с помощью вычисления значения 
многочлена методом Горнера.
Начальный размер таблицы должен быть равным 8-ми. 
Перехеширование выполняйте при добавлении элементов в случае, когда 
коэффициент заполнения таблицы достигает 3/4.
Структура данных должна поддерживать операции добавления строки в 
множество, удаления строки из множества и проверки принадлежности 
данной строки множеству.
1_1. Для разрешения коллизий используйте квадратичное пробирование. 
i-ая проба g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.
*/



#include <iostream>
#include <vector>
#include <cassert>
#include <sstream>
#include <iterator>
#include <numeric>

class StringHasher {
public:
    unsigned int operator()( const std::string& str ) const {
        unsigned int hash = 0;
        const unsigned int mult = 137;
        for (auto sym : str) {
            hash = hash * mult + sym;
        }
        return hash;
    }
};

template <class T, class H>
class HashTable {
public:
    HashTable(int initSize, const H& hasher);
    ~HashTable();

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
HashTable<T, H>::HashTable(int initSize, const H& hasher):  
    hasher(hasher),
    keysCount(0),
    table(initSize) {}

template <class T, class H>
HashTable<T, H>::~HashTable() {}

template <class T, class H>
bool HashTable<T, H>::Has(const T& key) const {
    unsigned int hash = hasher(key) % table.size();
    int iterations = 0;
    while (table[hash].getState() != States::notUsed) {
        ++iterations;
        if (iterations > table.size()) {
            return false;
        }
        if (table[hash].getState() == States::used && table[hash].key == key) {
            return true;
        }
        hash = getNextProb(hash, iterations);
    }
    return false;

}

template <class T, class H>
bool HashTable<T, H>::Add(const T& key) {
    if (keysCount * 4 >= table.size() * 3) {
        growTable();
    }
    unsigned int fullHash = hasher(key);
    unsigned int hash = fullHash % table.size();
    
    int cnt = 1;
    while (table[hash].getState() == States::used) {
        if (table[hash].key == key) return false;
        hash = getNextProb(hash, cnt);
        ++cnt;
    }
    table[hash].AddValue(key, fullHash);
    ++keysCount;
    return true;
}

template <class T, class H>
bool HashTable<T, H>::Delete(const T& key) {
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
        hash = getNextProb(hash, iterations);
    }
    return false;
}


template <class T, class H>
void HashTable<T, H>::growTable() {
    unsigned int newTableSize = 2 * table.size();
    std::vector <HashTableNode> oldTable = std::move(table);
    std::vector <HashTableNode> newTable(newTableSize);
    table = std::move(newTable);

    for (auto hashTableNode : oldTable) {
        if (hashTableNode.mark != States::used) {
            continue;
        }
        unsigned int hash = hashTableNode.hash % newTableSize;
        int cnt = 0;
        while (table[hash].getState() == States::used) {
            hash = getNextProb(hash, cnt);
            ++cnt;
        }
        table[hash] = hashTableNode;
    }
}

template <class T, class H>
unsigned int HashTable<T, H>::getNextProb(const unsigned int prevValue, 
                                          const unsigned int i) const {
    return (prevValue + i) % table.size();
}





void run(std::istream &input, std::ostream &output) {
    StringHasher hasher;
    HashTable <std::string, StringHasher> hashTable(8, hasher);

    char operation;
	std::string word;
    while( input >> operation >> word ) {
		if( operation == '+' )
			output << (hashTable.Add( word ) ? "OK" : "FAIL") << std::endl;
		if( operation == '?' )
			output << (hashTable.Has( word ) ? "OK" : "FAIL") << std::endl;
		if( operation == '-' )
			output << (hashTable.Delete( word ) ? "OK" : "FAIL") << std::endl;
	}

}



int main() {
    // test();
    run(std::cin, std::cout);
    return 0;
}
