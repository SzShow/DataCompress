
#include "LZ77CodeGen.h"

template<class T, class Code, template<class, class> class SW_SearchType>
bool LZ77Encode(const std::vector<T>& data,
								std::vector<code>* packed,
								size_t indexBit,
								SW_SearchType<typename std::vector<T>::const_iterator, code>& swSearch)
{
	const size_t szCode = sizeof(Code) / sizeof(char);
	const size_t bitCode = std::numeric_limits<Code>::digits;
	const size_t szValue = sizeof(T) / sizeof(char);
	const size_t bitValue = std::numeric_limits<T>::digits;

	if (&data == 0 || packed == 0) 
	{
		return(false);
	}

	if (indexBit >= bitCode)
	{
		return(false);
	}

	size_t countBit = bitCode - indexBit;
	Code maxWinSize = pow(2, indexBit) - 1;
	Code maxCountSize = pow(2, countBit) - 1;

	LZ77_CodeGenerator<Code> codeGen(indexBit, countBit);
	SlidingWindow<typename std::vector<T>::const_iterator, Code>
		sWin(data, maxWinSize, maxCountSize);
	
	Code cacheCode = 0;
	size_t cacheLen = 0;
	Code index = 0;
	Code count = 0;

	packed->clear();
	while(! sWin.empty())
	{
		if (count * szValue > szCode)
		{
			WriteToArray(static_cast<Code>(1), static_cast<size_t>(1), 
				&cacheCode, &cacheLen, packed);
			WriteToArray(codeGen.Encode(index, count), 
				codeGen.bits(), &cacheCode, &cacheLen, packed);
			sWin.inc(count);
		}
		else
		{
			WriteToArray(static_cast<Code>(0), static_cast<size_t>(1), 
				&cacheCode, &cacheLen, packed);
			WriteToArray(sWin.value(), bitValue, &cacheCode, &cacheLen, packed);
			sWin.inc(1);
		}
		
	}

	if (cacheLen > 0)
	{
		packed -> push_back( cacheCode << (bitCode - cacheLen));

		return(true);
	}
}


