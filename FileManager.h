
//---------------------------------------------------------------------------
// Developed by Shota Suzuki
//---------------------------------------------------------------------------
/**
* @file FileManager.h
* @brief ファイル入出力
* @todo stringのメモリ開放, 圧縮ファイルの圧縮ブロックの追加
* @note singletonデザインパターンに従って記述。
*
* 　詳細説明
*
* LZ77方式によって入力されたファイルを圧縮するプログラムです。
* コマンドライン引数に入力ファイル名を入れると、
* 対応するファイルの圧縮・解凍ファイルが出力されます
* 圧縮ファイルには独自の形式として.cmpを用いてます。
*
* 現在、ファイルの読み込みが全体の中での途中までしか行かない
* 不具合が確認されています。
* 実際に発生したのは提出ファイルの中に添付したTestImage.cmpおよび
* TestImage.jpgを圧縮して得られるファイルです。
* LoadFile()内にあるifstream辺りの操作が意図した通りに動いていない点まで
* 確認しましたが、それ以上の事はこちらでは分かりませんでした。
*
* singletonインスタンス生成はCreate()
* singletonインスタンス呼び出しはl_pInstance
* singletpnインスタンス消去はDestroy()
*/
//---------------------------------------------------------------------------

#ifndef FILE_MANAGER_H_
#define FILE_MANAGER_H_

#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>

class FileManager
{
public:
	static FileManager* GetInstance()
	{
		return l_pInstance;
	}


	static void Create()
	{
		if (!l_pInstance)
		{
			l_pInstance = new FileManager;
		}
	}
	static void Destroy()
	{
		l_pInstance->_basicName.clear();
		l_pInstance->_basicName.shrink_to_fit();
		l_pInstance->_extensionName.clear();
		l_pInstance->_extensionName.shrink_to_fit();

		delete l_pInstance;
		l_pInstance = nullptr;
	}

	int LoadFile(const char* fileName, std::string* out);
	int SaveFile(const std::string* fileName, const std::string* extension, std::string* in);

	std::string GetBasicName()
	{
		return _basicName;
	}
	std::string GetExtensionName()
	{
		return _extensionName;
	}


protected:
	FileManager();
	~FileManager();

	bool SaveFileName(const char* fileName);

protected:
	static FileManager* l_pInstance;

	size_t _inputSize;
	size_t _outputSize;

	std::string _basicName;
	std::string _extensionName;

};

#endif//FILE_MANAGER_H_


