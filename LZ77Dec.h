
#include "LZ77CodeGen.h"

template<class T, class Code>
bool LZ77Decode(const std::vector<Code>& packed, std::vecor<T>* data,
								size_t indexBit, typename std::vector<T>::size_type dataCount)
{
	const size_t szCode = sizeof(Code) / sizeof(char);
	const size_t bitCode = std::numeric_limits<Code>::digits;
	const size_t bitValue = std::numeric_limits<T>::digits;

	if (&packed == 0 || data == 0) return(false);

	if(indexBit + szCode >= bitCode)
		return(false);

	size_t countBit = bitCode - indexBit;

	Code cacheCode = 0;
	size_t cacheLen = 0;
	Code code;
	Code index = 0;
	Code count = 0;
	T isCode;
	T t;

	LZ77_CodeGenerator<Code>::const_iterator cit = packed.begin();
	typename std::vector<Code>::const_iterator cit = packed.begin();
	typename std::vector<T>::size_type st = 0;

	while (st < dataCount)
	{
		if (!ReadFromArray(&cit, packed.end(), &cacheCode, &cacheLen, &isCode, static_cast<size_t>(1))
		{
			return(false);
		}

		if (isCode != 0)
		{
			if (!ReadFromArray(&cit, packed.end(), &cacheCode, &cacheLen, &code, ))
			{
				return(false);
			}
			codeGen.decode(code, &index, &count);
			typename std::vector<T>::size_type read = data->size() - index;
			for (size_t sz = 0; sz < count; ++sz)
			{
				data -> push_back((*data)[read]);
				++read;
			}
			st += count;
		}
		else
		{
			if (!ReadFromArray(&cit, packed.end(), &cacheCode, &cacheLen, &t, bitValue))
			{
				return(false);
				data -> push_back(t);
				++st;
			}
		}
		
		return(true);
	}
}
