
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

		delete l_pInstance;
		l_pInstance = nullptr;
	}

protected:
	// コンストラクタ・デコンストラクタ
	LZ77Decompresser(const std::string input);
	~LZ77Decompresser();


	// 内部機能
	void SplitCompressed();
	void SplitCompressElements(const int splitedIndex);
	std::string Decode();


protected:
	// シングルトンインスタンス
	static LZ77Decompresser* l_pInstance;

	// 内部変数
	std::string _input; // 入力データ
	std::vector<std::string> _splited;
	int _codeIndex;
	int _codeLength;
	std::string _codeLastChar;
	std::string _pastOutput;

};


#endif
