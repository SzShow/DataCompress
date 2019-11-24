
#ifndef LZ77_COMPRESSER_H_
#define LZ77_COMPRESSER_H_

#include <string>

#include "UtilityFunction.h"

std::string Compress(const std::string input);

class LZ77Compresser
{
public:
	LZ77Compresser(const std::string input);
	~LZ77Compresser();
	std::string Compress(const std::string input);

protected:
	void EncodeChar();
	void ScanMatchingChar(const int currentIndex);
	void AssignValueToBuffer(int* currentIndex);
	void SetResultArries(const int target);
	void ResetBufferValue();
	std::string ConstructCode(const int target);

	void  GetIndexOf();

protected:
	string _input;
	int _inputLength;
	int _indexOfMaxLength;

	int* _resultMatchingIndex;
	int* _resultMatchingLength;
	int* _resultLastChar;

	int* _bufferMatchingIndex;
	int* _bufferMatchingLength;
	int* _bufferLastChar;
};


LZ77Compresser::~LZ77Compresser()
{
}

#endif
