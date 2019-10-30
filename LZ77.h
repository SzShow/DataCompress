
#include <vector>

// Encoder
template<class T, class Code, template<class, class> class SW_SearchType>
bool LZ77Encode(const std::vector<T>& data,
								std::vector<Code>* packed,
								size_t indexBit,
								SW_SearchType<typename std::vector<T>::const_iterator, Code>& swSearch);

// Decoder
template<class T, class Code>
bool LZ77Decode(const std::vector<Code>& packed, std::vector<T>* data,
								size_t indexBit, typename std::vector<T>::size_type dataCount);

// CodeGenerator
template<class Code>
class LZ77_CodeGenerator 
{
private:
	size_t indexBit_;
	size_t countBit_;

	Code indexMask_;
	Code countMask_;

public:
	LZ77_CodeGenerator(size_t indexBit, size_t countBit)
		: indexBit_(indexBit), countBit_(countBit)
	{
		indexMask_ = ~(~(static_cast<Code>(0)) << indexBit_);
		countMask_ = ~(~(static_cast<Code>(0)) << indexBit_);
	}
	
	Code Encode (Code index, Code count) const
	{
		return((index << countBit_) | count);
	}

	void Decode(Code code, Code* index, Code* count) const
	{
		*index = (code >> countBit_) & indexMask_;
		*count = code & countMask_;
	}
	
	size_t bits() const
	{
		return (indexBit_ + countBit_);
	}

};


#pragma region SlidingWindow

template<class In, class Code>
struct SW_Search
{
	void operator()(In dataStart, In dataEnd, Code winSize, Code maxCountSize, Code* index, Code* count);
};


template<class In, class Code>
void SW_Search<In, Code>::operator()(In dataStart, In dataEnd, Code winSize, Code maxCountSize, Code* index, Code* count);


template<class In, class Code>
class SlidingWindow
{
private:
	In dataStart_;
	In dataEnd_;
	In curIndex_;
	Code winSize_;
	Code maxWinSize_;
	Code maxCountSize_;

public:
	// 通常コンストラクタ
	SlidingWindow(In s, In e, Code maxWinSize, Code maxCountSize)
		:dataStart_(s), dataEnd_(e), curIndex_(s),
		winSize_(0), maxWinSize_(maxWinSize), maxCountSize_(maxCountSize)
	{
		if (std::distance(s, e) < maxWinSize_)
		{
			maxWinSize_ = std::distance(s, e);
		}
	}

	// データ列Containerを指定した場合のコンストラクタ
	template<class Container>
	SlidingWindow(const Container& container, Code maxWinSize, Code maxCountSize)
		: dataStart_(container.begin()), dataEnd_(container.end()), curIndex_(container.begin()),
		winSize_(0), maxWinSize_(maxWinSize_), maxCountSize_(maxCountSize_)
	{

	}

	template<template<class, class> class SW_Search>
	void search(SW_Search<In, Code>& search, Code* index, Code* count) const;

	value_type value() const;

	bool empty() const;

	void inc(size_t n);
};

template<class In, class Code>
void SlidingWindow<In, Code>::inc(size_t n);

#pragma endregion
