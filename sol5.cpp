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

	void WriteBit( bool bit );
	void WriteByte( unsigned char byte );

	const vector<unsigned char> GetBuffer() const { return buffer; }
    const unsigned int GetFreeBits() const { return (buffer.size() * 8) - bitsCount; }

private:
	vector<unsigned char> buffer;
	int bitsCount;
};

void OutBitStream::WriteBit( bool bit ) {
	if( bitsCount % 8 == 0 ) {
		buffer.push_back( 0 );
	}

	if( bit ) {
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

void ShowSym(byte sym) {
    for (int i = 0; i < 8; ++i) {
        cout << ((sym & (1 << i)) ? "1" : "0");
    }
}

class Packer {
private:
    struct ServiceInfo {
        byte sizeOfServiceInfo; // in bytes
        byte freeBitsAfterInfo;
        byte freeBits;
        vector <byte> createVector() {
            vector <byte> ans;
            ans.push_back(sizeOfServiceInfo);
            ans.push_back(freeBitsAfterInfo);
            ans.push_back(freeBits);
            return ans;
        }
    };
    vector <byte> packedInfo;

    void AddVectorToAns(vector <byte>& vect) {
        for (auto elem : vect) {
            packedInfo.push_back(elem);
        }
    }
public:
    Packer() {}

    vector <byte> Pack(OutBitStream& table, OutBitStream& info) {
        ServiceInfo servInfo;
        vector <byte> tableInfo = table.GetBuffer();
        servInfo.sizeOfServiceInfo = tableInfo.size();
        servInfo.freeBitsAfterInfo = table.GetFreeBits();
        servInfo.freeBits = info.GetFreeBits();
        vector <byte> servInfoVect = servInfo.createVector();
        AddVectorToAns( servInfoVect );
        AddVectorToAns( tableInfo );
        vector <byte> infoBuffer = info.GetBuffer();
        AddVectorToAns( infoBuffer );
        return packedInfo;
    }

    void ShowPacked(bool byBytes = true) {
        for (auto sym : packedInfo) {
            ShowSym(sym);
            if (byBytes) {
                cout << " ";
            }
        }
    }
};

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

    vector <byte> PackInfo() {
        OutBitStream table = SmartPackTree();
        // table.WriteBit(true);
        OutBitStream info;
        info.WriteBit(true);
        Packer packer;
        vector <byte> packed(packer.Pack(table, info));
        packer.ShowPacked();
        return packed;
    }

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
    void dfsSmartPacker(Node* node, OutBitStream& output) {
        if (node->isList) {
            output.WriteBit(1);
            output.WriteByte(node->sym);
        } else {
            dfsSmartPacker(node->left, output);
            dfsSmartPacker(node->right, output);

            output.WriteBit(0);
        }
        return;
    }
    OutBitStream SmartPackTree() {
        OutBitStream output;
        Node* root = pq.top();
        dfsSmartPacker(root, output);
        return output;
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
    ht.PackInfo();
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
    s = "abc";
    // cout << (int)'a' << endl;
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