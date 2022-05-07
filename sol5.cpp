#include "Huffman.h"
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <string>
#include <cassert>

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
void ShowSym(byte sym) {
    for (int i = 0; i < 8; ++i) {
        cout << ((sym & (1 << i)) ? "1" : "0");
    }
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


class InpBitStream {
public:
	InpBitStream(vector <byte> bytes) : buffer(bytes), bitsRead(0) {}

	bool ReadBit();
	byte ReadByte();
    bool isNotEnd() { return bitsRead < buffer.size() * 8; }
    unsigned int GetSize() { return buffer.size(); }
	// const vector<unsigned char> GetBuffer() const { return buffer; }
    // const unsigned int GetFreeBits() const { return (buffer.size() * 8) - bitsCount; }

private:
	vector<unsigned char> buffer;
	int bitsRead;
};

bool InpBitStream::ReadBit() {
    assert(isNotEnd());
    bool ans = (buffer[bitsRead / 8] & (1 << (bitsRead % 8)));
    ++bitsRead;
    return ans;
}

byte InpBitStream::ReadByte() {
    assert(isNotEnd());
    byte ans = 0;
    unsigned int offset = bitsRead % 8;
    unsigned int alreadyRead = 8 - offset;
    ans |= (buffer[bitsRead / 8] >> offset);
    bitsRead += alreadyRead;
    ans |= (buffer[bitsRead / 8] << alreadyRead);
    bitsRead += offset;
    return ans;

}

struct ServiceInfo {
    unsigned int sizeOfServiceInfo; // in bytes
    byte freeBitsAfterInfo;
    byte freeBits;
    vector <byte> createVector() {
        vector <byte> ans;
        byte tmp = 0;
        ans.push_back((tmp | sizeOfServiceInfo));
        ans.push_back((tmp | (sizeOfServiceInfo >> 8)));
        ans.push_back(freeBitsAfterInfo);
        ans.push_back(freeBits);
        return ans;
    }
    ServiceInfo() : sizeOfServiceInfo(0), freeBitsAfterInfo(0), freeBits(0) {}
    ServiceInfo(InpBitStream &inpBitStream) {
        byte first = inpBitStream.ReadByte();
        byte second = inpBitStream.ReadByte();
        sizeOfServiceInfo = first;
        sizeOfServiceInfo |= (second << 8);
        freeBitsAfterInfo = inpBitStream.ReadByte();
        freeBits = inpBitStream.ReadByte();
    }
    void ShowInfo() {
        cout << "SHOW SERVICE INFO\n";
        cout << sizeOfServiceInfo << endl;
        cout << (int)freeBitsAfterInfo << endl;
        cout << (int)freeBits << endl;
        cout << "END SERVICE INFO\n";
    }
};

class Packer {
private:
    
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
    HuffmanTree(vector <byte> bytes) : root(nullptr) {
        baseInfo = bytes;
        for (auto word : bytes) {
            freqTable[word] += 1;
        }
    }
    HuffmanTree(InpBitStream inpStream) {
        ServiceInfo servInfo(inpStream);
        servInfo.ShowInfo();
    }
    void BuildTree() {
        initPriorityQueue();
        BuildTreeByPQ();
    }
    void ShowTree();

    vector <byte> PackInfo() {
        OutBitStream table = SmartPackTree();
        // table.WriteBit(true);
        OutBitStream info = ArchiveInfo();
        // vector <byte> archivedInfo = info.GetBuffer();
        // cout << "DEBUG: archived info:\n";
        // for (auto sym : archivedInfo) {
        //     cout << (int)sym << " ";
        // }
        // cout << endl;
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
    vector <byte> baseInfo;
    map < byte, vector <unsigned int> > codes;
    Node* root;

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
    
    void dfs(Node* node, vector <unsigned int> &code, bool print = false) {
        if (node->isList) {
            if (print) {
                cout << node->sym << " freq: " << node->freq << ": ";
                printVector(code);
            } else {
                codes[node->sym] = code;
            }
            // 
            return;
        } else {
            code.push_back(0);
            dfs(node->left, code, print);
            code.pop_back();

            code.push_back(1);
            dfs(node->right, code, print);
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

    OutBitStream ArchiveInfo() {
        Node* root = pq.top();
        vector <unsigned int> st;
        OutBitStream archive;

        // make codes of symbols
        dfs(root, st);

        for (auto sym : baseInfo) {
            for (auto bit : codes[sym]) {
                archive.WriteBit(bit);
            }
        }
        return archive;
    }
};
void HuffmanTree::ShowTree() {
    Node* root = pq.top();
    vector <unsigned int> code;
    dfs(root, code, true);
}

static void copyStream(IInputStream& input, IOutputStream& output) {
    byte value;
    while(input.Read(value)) {
        output.Write(value);
    }
}

void Encode(IInputStream& original, IOutputStream& compressed) {
    byte value;
    vector <byte> input;
    while(original.Read(value)) {
        input.push_back(value);
    }
    HuffmanTree ht(input);
    ht.BuildTree();
    ht.ShowTree();
    vector <byte> packed = ht.PackInfo();
    for (auto sym : packed) {
        compressed.Write(sym);
    }
    // for (auto sym : writer.GetBuffer()) {
    //     compressed.Write(sym);
    // }
    // copyStream(original, compressed);
}

void Decode(IInputStream& compressed, IOutputStream& original) {
    byte value;
    vector <byte> input;
    cout << "\ndeconding: \n";
    while(compressed.Read(value)) {
        ShowSym(value);
        cout << " ";
        input.push_back(value);
    }
    cout << "\nend of reading\n";
    InpBitStream reader(input);
    HuffmanTree ht(reader);
    // ht.BuildTree();
    // ht.ShowTree();
    // ht.PackInfo();
    // copyStream(compressed, original);
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
    for (auto sym : outVector) {
        cout << "sym: " << sym << " \n";
    }


    CInputStream lastOut(outVector);
    vector <byte> end;
    COutputStream outNew(end);
    Decode(lastOut, outNew);

    out.ShowData();





    // test InpBitStream
    // vector <byte> inpVector;
    // vector <unsigned int> vect = {255, 255};
    // inpVector.push_back(128);
    // inpVector.push_back(255);
    // InpBitStream inpBit(inpVector);
    // cout << "test InpBitStream\n";
    // cout << inpBit.ReadBit() << " ";
    // cout << inpBit.ReadBit() << " ";
    // ShowSym(inpBit.ReadByte());
    return 0;
}