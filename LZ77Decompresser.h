
//---------------------------------------------------------------------------
// Developed by Shota Suzuki
//---------------------------------------------------------------------------
/**
* @file LZ77Decompresser.h
* @brief ファイル解凍
* @todo 解凍の際にエラーの修正
* @note singletonデザインパターンに従って記述。
*
* 　詳細説明
*
* LZ77方式によって入力されたファイルを解凍するクラスです。
* .cmpファイル内の先頭から拡張子を抽出した後、
* ファイル内にある符号を全て読み取り、
* それらを抽出された順番から復号して解凍する仕組みとなっています。
* 
*
* singletonインスタンス生成はCreate()
* singletonインスタンス呼び出しはl_pInstance
* singletpnインスタンス消去はDestroy()
*/
//---------------------------------------------------------------------------

#ifndef LZ77_DECOMPRESSER_H_
#define LZ77_DECOMPRESSER_H_

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
//#include <memory>


class LZ77Decompresser
{
public:

	// 圧縮実行のインターフェイス
	int Decompress(std::string* output, std::string* extension);


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
		l_pInstance->_input.clear();
		l_pInstance->_input.shrink_to_fit();
		l_pInstance->_codes.clear();
		l_pInstance->_codes.shrink_to_fit();
		l_pInstance->_codeLastChar.clear();
		l_pInstance->_codeLastChar.shrink_to_fit();
		l_pInstance->_pastOutput.clear();
		l_pInstance->_pastOutput.shrink_to_fit();

		delete l_pInstance;
		l_pInstance = nullptr;
	}

protected:
	// コンストラクタ・デコンストラクタ
	LZ77Decompresser(const std::string input);
	~LZ77Decompresser();


	// 内部機能
	//void ExtractExtension();
	void SplitCompressed(std::string* extension);
	int SplitCompressElements(const int splitedIndex);
	void Decode();


protected:
	// シングルトンインスタンス
	static LZ77Decompresser* l_pInstance;

	// 内部変数
	std::string _input; // 入力データ
	std::vector<std::string> _codes;
	int _codeIndex;
	int _codeLength;
	std::string _codeLastChar;
	std::string _pastOutput;

};


#endif
