
//---------------------------------------------------------------------------
// Developed by Shota Suzuki
//---------------------------------------------------------------------------
/**
* @file LZ77Decompresser.h
* @brief ファイル圧縮
* @note singletonデザインパターンに従って記述。
*
* 　詳細説明
*
* LZ77方式によって入力されたファイルを圧縮するクラスです
* 一致文字列の開始位置(index)、一致文字列の長さ(length)などの符号化に必要な情報を集めた後に
* [index length 符号化部のすぐ後に続く文字]より表される3Byteの符号を出力します。
* また、一致文字列が見当たらない部分については符号化しません。
*
* SlidingWindowの位置と長さについては、それぞれ最大でUCHAR_MAXまでの値を取れます。
*
* singletonインスタンス生成はCreate()
* singletonインスタンス呼び出しはl_pInstance
* singletpnインスタンス消去はDestroy()
*/
//---------------------------------------------------------------------------

#ifndef LZ77_COMPRESSER_H_
#define LZ77_COMPRESSER_H_

#include <string>


class LZ77Compresser
{
public:

	// 圧縮実行のインターフェイス
	int Compress(const std::string* extensionName, std::string* output);

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

		delete[] l_pInstance->_resultMatchingIndex;
		delete[] l_pInstance->_resultMatchingLength;
		delete[] l_pInstance->_resultLastChar;
		delete[] l_pInstance->_bufferMatchingIndex;
		delete[] l_pInstance->_bufferMatchingLength;
		delete[] l_pInstance->_bufferLastChar;

		l_pInstance->_input.clear();
		l_pInstance->_input.shrink_to_fit();

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
	void PickupLastChar();
	void SetResultBuffer();
	void ResetBufferValue();
	inline void DecideNextIndex();
	int SearchLongestIndex();
	std::string Encode(const int targetIndex);

protected:
	// シングルトンインスタンス
	static LZ77Compresser* l_pInstance;

	// 内部変数
	std::string _input; // 入力データ
	int _inputLength; // 入力データの要素数
	int _currentIndex; // 符号化の対象となっている文字列の最後尾
	int _largestIndex; // 最も長い一致文字列の開始位置
	int _resultCount; // 最後に書き込んだ出力用バッファのインデックス

	// 出力バッファ
	unsigned char* _resultMatchingIndex;
	unsigned char* _resultMatchingLength;
	int* _resultLastChar;

	// 基本バッファ
	unsigned char* _bufferMatchingIndex;
	unsigned char* _bufferMatchingLength;
	int* _bufferLastChar;
};


#endif
