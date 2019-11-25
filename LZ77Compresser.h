
#ifndef LZ77_COMPRESSER_H_
#define LZ77_COMPRESSER_H_

#include <string>

#include "UtilityFunction.h"

std::string Compress(const std::string input);

class LZ77Compresser
{
public:

	std::string Compress(const std::string input);


	static void Create()
	{
		if (!l_pInstance)
		{
			l_pInstance = new LZ77Compresser;
		}
	}
	static void Destroy()
	{
		delete l_pInstance;
		l_pInstance = nullptr;
	}

protected:
	LZ77Compresser(const std::string input);
	~LZ77Compresser();

	void EncodeChar();
	void ScanMatchingChar(const int currentIndex);
	void AssignValueToBuffer(int* currentIndex);
	void SetResultArries(const int target);
	void ResetBufferValue();
	std::string ConstructCode(const int target);

	void  GetIndexOf();

protected:
	static LZ77Compresser* l_pInstance;

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
