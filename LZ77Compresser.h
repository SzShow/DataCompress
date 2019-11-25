
#ifndef LZ77_COMPRESSER_H_
#define LZ77_COMPRESSER_H_

#include <string>

#include "UtilityFunction.h"


class LZ77Compresser
{
public:

	// 圧縮実行のインターフェイス
	int Compress(std::string* output);

	// インスタンスの取得
	static LZ77Compresser* GetInstance()
	{
		return l_pInstance;
	}

	// インスタンスの操作
	static void Create(const std::string input)
	{
		if (!l_pInstance)
		{
			l_pInstance = new LZ77Compresser(input);
		}
	}
	static void Destroy()
	{
		delete l_pInstance;
		l_pInstance = nullptr;
	}

protected:
	// コンストラクタ・デコンストラクタ
	LZ77Compresser(const std::string input);
	~LZ77Compresser();

	
	// 内部機能
	void ScanMatchingChar();
	void CountMatchingLength();
	int PickupLastChar();
	void SetResultBuffer();
	void ResetBufferValue();
	inline void DecideNextIndex();
	std::string Encode(const int targetIndex);
	int SearchLongestIndex();

protected:
	// シングルトンインスタンス
	static LZ77Compresser* l_pInstance;

	// 内部変数
	string _input; // 入力データ
	int _inputLength; // 入力データの要素数
	int _currentIndex; // 符号化の対象となっている文字列の最後尾
	int _largestIndex; // 最も長い一致文字列の開始位置
	int _resultCount; // 最後に書き込んだ出力用バッファのインデックス

	// 出力バッファ
	int* _resultMatchingIndex;
	int* _resultMatchingLength;
	int* _resultLastChar;

	// 基本バッファ
	int* _bufferMatchingIndex;
	int* _bufferMatchingLength;
	int* _bufferLastChar;
};


#endif
