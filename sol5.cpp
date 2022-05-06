#include "Huffman.h"
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <string>

using std::vector;
using std::map;
using std::priority_queue;
using std::endl;
using std::cout;
using std::string;

template <class T>
void printVector(const vector <T>& vect) {
    for (auto& elem : vect) {
        cout << elem << " ";
    }
    cout << endl;
}

class OutBitStream {
public:
	OutBitStream() : bitsCount( 0 ) {}

	void WriteBit( unsigned char bit );
	void WriteByte( unsigned char byte );

	const vector<unsigned char>& GetBuffer() const { return buffer; }
    const unsigned int GetFreeBits() const { return (buffer.size() * 8) - bitsCount; }

private:
	vector<unsigned char> buffer;
	int bitsCount;
};

void OutBitStream::WriteBit( unsigned char bit ) {
	if( bitsCount % 8 == 0 ) {
		buffer.push_back( 0 );
	}

	if( bit != 0 ) {
		int bitPos = bitsCount % 8;
		buffer[bitsCount / 8] |= 1 << bitPos;
	}

	++bitsCount;
}

void OutBitStream::WriteByte( unsigned char byte ) {
	if( bitsCount % 8 == 0 ) {
		buffer.push_back( byte );
	} else {
		int offset = bitsCount % 8;
		buffer[bitsCount / 8] |= byte << offset;
		buffer.push_back( byte >> (8 - offset) );
	}

	bitsCount += 8;
}


class HuffmanTree {
public:
    HuffmanTree(vector <byte> bytes) {
        for (auto word : bytes) {
            freqTable[word] += 1;
        }
    }
    void BuildTree() {
        initPriorityQueue();
        BuildTreeByPQ();
    }
    void ShowTree();

private:
    struct Node {
        Node* left;
        Node* right;
        byte sym;
        bool isList;
        unsigned int freq;
        Node(byte _sym) : sym(_sym), left(nullptr), right(nullptr), freq(0), isList(true) {}
        Node(byte _sym, unsigned int _freq) : sym(_sym), left(nullptr), right(nullptr), freq(_freq), isList(true) {}
        Node(unsigned int _freq, Node* _left, Node* _right) : left(_left), right(_right), freq(_freq), isList(false) {}
    };
    class isGreater {
    public:
        bool operator()(const Node* left, const Node* right) {
            return (left->freq) > (right->freq);
        }
    };
    map <byte, unsigned int> freqTable;
    priority_queue <Node*, vector <Node*>, isGreater> pq;

    void initPriorityQueue() {
        for (auto symWithFreq : freqTable) {
            byte sym = symWithFreq.first;
            byte freq = symWithFreq.second;
            Node* newNode = new Node(sym, freq);
            pq.push(newNode);
        }
    }
    void BuildTreeByPQ() {
        while (pq.size() > 1) {
            Node* node1 = pq.top();
            pq.pop();
            Node* node2 = pq.top();
            pq.pop();
            int newFreq = node1->freq + node2->freq;
            Node* newNode = new Node(newFreq, node1, node2);
            pq.push(newNode);
        }
    }
    
    void dfs(Node* node, vector <unsigned int> &code) {
        if (node->isList) {
            cout << node->sym << " freq: " << node->freq << ": ";
            printVector(code);
            return;
        } else {
            code.push_back(0);
            dfs(node->left, code);
            code.pop_back();

            code.push_back(1);
            dfs(node->right, code);
            code.pop_back();
        }
        return;
    }
};
void HuffmanTree::ShowTree() {
    Node* root = pq.top();
    vector <unsigned int> code;
    dfs(root, code);
}

static void copyStream(IInputStream& input, IOutputStream& output) {
    byte value;
    while(input.Read(value)) {
        output.Write(value);
    }
}

void Encode(IInputStream& original, IOutputStream& compressed) {
    OutBitStream writer;
    byte value;
    vector <byte> input;
    while(original.Read(value)) {
        input.push_back(value);
    }
    HuffmanTree ht(input);
    ht.BuildTree();
    ht.ShowTree();
    // for (auto sym : writer.GetBuffer()) {
    //     compressed.Write(sym);
    // }
    // copyStream(original, compressed);
}

void Decode(IInputStream& compressed, IOutputStream& original) {
    copyStream(compressed, original);
}



int main() {
    vector <byte> inpVector;
    string s = "aaaaaabccccasdf";
    for (auto sym : s) {
        inpVector.push_back(sym);
    }
    CInputStream inp(inpVector);

    vector <byte> outVector;
    COutputStream out(outVector);
    Encode(inp, out);
    out.ShowData();
    return 0;
}