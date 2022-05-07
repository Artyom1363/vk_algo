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

const bool DEBUG = false;

const unsigned int TIMES_CODING = 2;

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
    InpBitStream() {}

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
    assert(bitsRead + 8 <= buffer.size() * 8);
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
    unsigned int dataBitsCount;
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
        dataBitsCount = inpBitStream.GetSize() * 8 - sizeOfServiceInfo * 8 - freeBits - 32;
    }
    unsigned int GetDataBitsCount() {
        return dataBitsCount;
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
    HuffmanTree(InpBitStream _inpStream) : inpStream(_inpStream) {
        ServiceInfo servInfo(inpStream);
        maxBits = servInfo.GetDataBitsCount();
        unsigned int tableSizeBits = servInfo.sizeOfServiceInfo * 8 - servInfo.freeBitsAfterInfo;
        RecoveryTree(tableSizeBits, inpStream);
        for (int i = 0; i < servInfo.freeBitsAfterInfo; ++i) inpStream.ReadBit();
        // ShowTree();
        // servInfo.ShowInfo();
    }

    void RecoveryTree(unsigned int tableSizeBits, InpBitStream& inpStream) {
        vector <Node*> st;
        unsigned int bitRead = 0;

        while (bitRead < tableSizeBits) {
            bool type = inpStream.ReadBit();
            ++bitRead;
            if (type) {
                byte sym = inpStream.ReadByte();
                Node* newNode = new Node(sym);
                st.push_back(newNode);
                bitRead += 8;
            } else {
                Node* first = st.back();
                st.pop_back();
                // assert(st.size() != 0);
                Node* second;
                if (st.size() == 0) {
                    second = nullptr;
                    std::swap(first, second);
                } else {
                    second = st.back();
                    st.pop_back();
                }

                Node* newNode = new Node(second, first);
                st.push_back(newNode);
            }
        }
        assert(st.size() == 1);
        root = st.back();
    }

    vector <byte> Unpack(bool debug = false) {
        // cout << "Unpack" << endl;
        vector <byte> ans;
        unsigned int bitsCounter = 0;
        // cout << "maxBits: " << maxBits << endl;
        if (DEBUG) {
            cout << "RECOVERED TREE:\n";
            ShowTree();
        }
        while (bitsCounter < maxBits) {
            Node* node = root;
            assert(node != nullptr);
            while (!(node->isList)) {
                bool bit = inpStream.ReadBit();
                ++bitsCounter;
                node = (bit ? node->right : node->left);
            }
            ans.push_back(node->sym);
        }
        // cout << "end of unpack" << endl;
        if (debug) {
            cout << "DEBUG: UnArchived info:\n";
            for (auto sym : ans) {
                cout << sym;
            }
            cout << "\nend of UnArchived info " << endl;
        }
        
        return ans;
    }

    void BuildTree() {
        initPriorityQueue();
        BuildTreeByPQ();
    }

    void ShowTree();

    vector <byte> PackInfo() {
        OutBitStream table = SmartPackTree();
        // table.WriteBit(true);
        // cout << "PackInfo" << endl;
        OutBitStream info = ArchiveInfo();
        // vector <byte> archivedInfo = info.GetBuffer();
        // cout << "DEBUG: archived info:\n";
        // for (auto sym : archivedInfo) {
        //     cout << (int)sym << " ";
        // }
        // cout << endl;
        Packer packer;
        vector <byte> packed(packer.Pack(table, info));
        // packer.ShowPacked();
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
        Node(Node* _left, Node* _right) : left(_left), right(_right), freq(0), isList(false) {}
    };
    class isGreater {
    public:
        bool operator()(const Node* left, const Node* right) {
            return (left->freq) > (right->freq);
        }
    };
    InpBitStream inpStream;
    map <byte, unsigned int> freqTable;
    priority_queue <Node*, vector <Node*>, isGreater> pq;
    vector <byte> baseInfo;
    map < byte, vector <unsigned int> > codes;
    Node* root;
    unsigned int maxBits;

    void initPriorityQueue() {
        for (auto symWithFreq : freqTable) {
            byte sym = symWithFreq.first;
            byte freq = symWithFreq.second;
            Node* newNode = new Node(sym, freq);
            pq.push(newNode);
        }
    }
    void BuildTreeByPQ() {
        if (pq.size() == 1) {
            Node* node = pq.top();
            root = new Node(node->freq, node, nullptr);
            return;
        }
        while (pq.size() > 1) {
            Node* node1 = pq.top();
            pq.pop();
            Node* node2 = pq.top();
            pq.pop();
            int newFreq = node1->freq + node2->freq;
            Node* newNode = new Node(newFreq, node1, node2);
            pq.push(newNode);
        }
        root = pq.top();
    }
    
    void dfs(Node* node, vector <unsigned int> &code, bool print = false) {
        if (node == nullptr) return;
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
        if (node == nullptr) return;
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
        dfsSmartPacker(root, output);
        return output;
    }

    OutBitStream ArchiveInfo() {
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
    // Node* root = pq.top();
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
    vector <byte> buffer;
    while(original.Read(value)) {
        buffer.push_back(value);
    }

    for (int i = 0; i < TIMES_CODING; ++i) {
        HuffmanTree ht(buffer);
        ht.BuildTree();
        buffer = ht.PackInfo();
        if (DEBUG) ht.ShowTree();
    }
    // vector <byte> archivedInfo = info.GetBuffer();
    if (DEBUG) {
        cout << "DEBUG: archived info:\n";
        for (auto sym : buffer) {
            ShowSym(sym);
            cout << " ";
        }
        cout << endl;
    }


    for (auto sym : buffer) {
        compressed.Write(sym);
    }
    // for (auto sym : writer.GetBuffer()) {
    //     compressed.Write(sym);
    // }
    // copyStream(original, compressed);
}

void Decode(IInputStream& compressed, IOutputStream& original) {
    byte value;
    vector <byte> buffer;
    while(compressed.Read(value)) {
        buffer.push_back(value);
    }
    for (int i = 0; i < TIMES_CODING; ++i) {
        InpBitStream reader(buffer);
        HuffmanTree ht(reader);
        buffer = ht.Unpack(DEBUG);
    }
    // cout << "\nend of reading\n";
    
    for (auto sym : buffer) {
        original.Write(sym);
    }

    // ht.BuildTree();
    // ht.ShowTree();
    // ht.PackInfo();
    // copyStream(compressed, original);
}



int main() {
    vector <byte> inpVector;
    string s = "aaaaaabccccasdfajf3j34";
    s = "a";
    s = "abc";
    for (auto sym : s) {
        inpVector.push_back(sym);
    }
    CInputStream inp(inpVector);
    

    vector <byte> outVector;
    COutputStream out(outVector);
    Encode(inp, out);


    CInputStream lastOut(outVector);
    vector <byte> end;
    COutputStream outNew(end);
    Decode(lastOut, outNew);

    // out.ShowData();





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