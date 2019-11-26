
#ifndef LZ77_DECOMPRESSER_H_
#define LZ77_DECOMPRESSER_H_

#include <string>
#include <vector>
#include <sstream>


class LZ77Decompresser
{
public:

	// 圧縮実行のインターフェイス
	int Decompress(std::string* output);

	// インスタンスの取得
	static LZ77Decompresser* GetInstance()
	{
		return l_pInstance;
	}

	// インスタンスの操作
	static void Create(const std::string input)
	{
		if (!l_pInstance)
		{
			l_pInstance = new LZ77Decompresser(input);
		}
	}
	static void Destroy()
	{

		// 出力バッファ
		delete[] l_pInstance->_resultMatchingIndex;
		delete[] l_pInstance->_resultMatchingLength;
		delete[] l_pInstance->_resultLastChar;

		// 基本バッファ
		delete[] l_pInstance->_bufferMatchingIndex;
		delete[] l_pInstance->_bufferMatchingLength;
		delete[] l_pInstance->_bufferLastChar;

		delete l_pInstance;
		l_pInstance = nullptr;
	}

protected:
	// コンストラクタ・デコンストラクタ
	LZ77Decompresser(const std::string input);
	~LZ77Decompresser();


	// 内部機能
	void SplitCompressed();
	void CountMatchingLength();
	void PickupLastChar();
	void SetResultBuffer();
	void ResetBufferValue();
	inline void DecideNextIndex();
	std::string Encode(const int targetIndex);
	int SearchLongestIndex();

protected:
	// シングルトンインスタンス
	static LZ77Decompresser* l_pInstance;

	// 内部変数
	std::string _input; // 入力データ
	std::vector<std::string> _splited;
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
