#pragma once


template<class Code>
class LZ77_CodeGenerator {
private:
	size_t indexBit_;
	size_t countBit_;

	Code indexMask_;
	Code countMask_;

public:
	LZ77_CodeGenerator(size_t indexBit, size_t countBit)
		: indexBit_(indexBit), countBit_(countBit);
	Code Encode (Code index, Code count);
	void decode(Code code, Code* index, Code* count);
	size_t bits();

};
