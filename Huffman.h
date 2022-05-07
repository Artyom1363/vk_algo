#pragma once

#include <vector>
#include <iostream>
typedef unsigned char byte;


struct IInputStream {
	// Возвращает false, если поток закончился
	virtual bool Read(byte& value) = 0;
};

struct IOutputStream {
	virtual void Write(byte value) = 0;
};



class CInputStream : public IInputStream {
public:
	CInputStream( const std::vector<byte>& _body ) : index( 0 ), body( _body ) {}

	virtual bool Read( byte& value );

private:
	unsigned int index;
	const std::vector<unsigned char>& body;
};

bool CInputStream::Read( byte& value )
{
	if( index >= body.size() ) {
		return false;
	}

	value = body[index++];
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Выходной поток
class COutputStream : public IOutputStream {
public:
	COutputStream( std::vector<byte>& _body ) : body( _body ) { body.clear(); }
	virtual void Write( byte value ) { 
        body.push_back( value ); 
    }

    void ShowData() {
        for (auto sym : body) {
            std::cout << sym;
        }
    }

private:
	std::vector<byte>& body;
};


// void Encode(IInputStream& original, IOutputStream& compressed);

// void Decode(IInputStream& compressed, IOutputStream& original);