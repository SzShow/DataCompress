
#include "LZ77.h"

// エンコードの実行
template<class T, class Code, template<class, class> class SW_SearchType>
bool LZ77Encode(const std::vector<T>& data,
	std::vector<Code>* packed,
	size_t indexBit, // 現在の位置情報
	SW_SearchType<typename std::vector<T>::const_iterator, Code>& swSearch)
{
	// 初期設定
	const size_t byteCode = sizeof(Code) / sizeof(char); // 出力データの型Codeのバイト数(sizeof(char)で割る理由は？)
	const size_t bitCode = std::numeric_limits<Code>::digits; // Codeのビット数
	const size_t byteValue = sizeof(T) / sizeof(char); // 入力データ型Tのバイト数
	const size_t bitValue = std::numeric_limits<T>::digits; // Tのビット数


	// エラーチェック
	if (&data == 0 || packed == 0) 
	{
		return(false);
	}
	if (indexBit >= bitCode)
	{
		return(false);
	}

	
	//エンコーダと窓の生成
	size_t countBit = bitCode - indexBit; // データの一致箇所のビット数
	Code maxWinSize = pow(2, indexBit) - 1; // どうして型はCode？
	Code maxCountSize = pow(2, countBit) - 1;

	LZ77_CodeGenerator<Code> codeGen(indexBit, countBit);
	SlidingWindow<typename std::vector<T>::const_iterator, Code>
		sWin(data, maxWinSize, maxCountSize); // どうして引数3つ？
	
	// 
	Code cacheCode = 0; 
	size_t cacheLen = 0; // キャッシュされた符号の長さ
	Code index = 0; // 窓の始点
	Code count = 0; // 一致したデータの長さ

	packed->clear();
	while(! sWin.empty())
	{
		// バイト数の削減見込みをチェック
		// 出力の型
		if (count * byteValue > byteCode)
		{
			//プレフィクスとして1書き込み
			WriteToArray(static_cast<Code>(1), static_cast<size_t>(1), 
				&cacheCode, &cacheLen, packed); 
			// 符号化データの書き込み
			WriteToArray(codeGen.Encode(index, count), 
				codeGen.bits(), &cacheCode, &cacheLen, packed); 
			sWin.inc(count);
		}
		else
		{
			//符号化せずにデータ書き込み
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


// Decoder
template<class T, class Code>
bool LZ77Decode(const std::vector<Code>& packed, std::vector<T>* data,
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


// CodeGenerator
template<class Code> // 原則としてCodeは符号なしの整数型
class LZ77_CodeGenerator {
private:
	size_t indexBit_;
	size_t countBit_;

	Code indexMask_;
	Code countMask_;

public:

	// それぞれのデータサイズ分、左から1を詰める
	LZ77_CodeGenerator(size_t indexBit, size_t countBit)
		: indexBit_(indexBit), countBit_(countBit)
	{
		indexMask_ = ~(~(static_cast<Code>(0)) << indexBit_);
		countMask_ = ~(~(static_cast<Code>(0)) << countBit_);
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


// SlidingWindow
template<class In, class Code>
void SW_Search<In, Code>::operator()(In dataStart, In dataEnd, Code winSize, Code maxCountSize, Code* index, Code* count)
{
	typedef typename std::iterator_traits<In>::difference_type difference_type;

	*index = *count = Code();

	In winStart = dataStart;
	std::advance(winStart, -static_cast<difference_type>(winSize));

	for(; winStart != dataStart; **winStart)
	{
		In winP = winStart;

		for (In dataP = dataStart; *winP == *dataP ; ++dataP)
		{
			if (dataP == dataEnd) break;
			++winP;
		}

		size_t sz = std::distance(winStart, winP);
		if (sz > *count)
		{
			*index = std::distance(winStart, dataStart);
			*count = sz;
		}

		if (*count > maxCountSize)
		{
			*count = maxCountSize;
			break;
		}

	}

}

template<class In, class Code, template<class, class> class SW_Search>
void SlidingWindow<In, Code>::search<SW_Search<In, Code>>(SW_Search<In, Code>& search,
	Code* index, Code* count) const
{
	search(curIndex_, dataEnd_, winSize_, maxCountSize_, index, count);
}

template<class In, class Code>
value_type SlidingWindow<In, Code>::value() const
{
	return(curIndex_ == dataEnd_);
}

template<class In, class Code>
void SlidingWindow<In, Code>::inc (size_t n)
{
	// 符号化開始点を一致したデータの長さ分だけ進める
	// または、符号化開始点を入力データ列の末尾にクリッピングする
	if (static_cast<size_t>(std::distance(curIndex_, dataEnd_)) < n)
	{
		curIndex_ = dataEnd_;
	}
	else
	{
		std::advance(curIndex_, n);
	}

	// 窓の長さが最大まで行っていなければ
	// 窓の長さを最大値を超えない範囲で伸ばす
	if(winSize_ < maxWinSize_)
	{
		winSize_ += n;
		if(winSize_ > maxWinSize_)
		{
			winSize_ = maxWinSize_;
		}
	}
}

// 符号化開始点が入力データ列の末尾にあればtrueを返す
template<class In, class Code>
bool SlidingWindow<In, Code>::empty() const
{
	return (curIndex_ == dataEnd_);
}


