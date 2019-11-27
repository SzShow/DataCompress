//---------------------------------------------------------------------------
// Developed by Shota Suzuki
//---------------------------------------------------------------------------
/**
* @file Main.cpp
* @brief 動作の基点部分
* @todo 今後追加する機能等(オプション)
* @note メモ(オプション)
*
* 　詳細説明
*
* LZ77方式によって入力されたファイルを圧縮するプログラムです。
* コマンドライン引数に入力ファイル名を入れると、
* 対応するファイルの圧縮・解凍ファイルが出力されます
* 圧縮ファイルには独自の形式として.cmpを用いてます。
* 
*/
//---------------------------------------------------------------------------

//Source: https://gist.github.com/MajedSiefALnasr/32cff52b01d764ee158f2c36e4e84d95

//#include <stdlib.h>
#include <iostream>
//#include <vector>
#include <sstream>

using namespace std;

#include "FileManager.h"
#include "LZ77Compresser.h"
#include "LZ77Decompresser.h"


// argv[0]: 入力ファイル名
// argv[1]: 出力ファイル名
int main(int argc, char *argv[])
{
    // 初期化
	int errorCode = 0;

	string input;
	string output;

	FileManager::Create();
	FileManager* fManager = FileManager::GetInstance();


	// ファイル読み込み
	
	errorCode = fManager->LoadFile(argv[1], &input);
	if (errorCode != 0)
	{
		return errorCode;
	}
	
	

	// 圧縮の実行
	// 
	//LZ77Compresser::Create(input);
	//LZ77Compresser* lz77 = LZ77Compresser::GetInstance();
	//errorCode = lz77->Compress(&output);
	LZ77Decompresser::Create(input);
	LZ77Decompresser* lz77 = LZ77Decompresser::GetInstance();
	errorCode = lz77->Decompress(&output);
	if (errorCode != 0)
	{
		return errorCode;
	}


	// ファイルの出力
	errorCode = fManager->SaveFile(argv[2], &output);
	if (errorCode != 0)
	{
		return errorCode;
	}

	//終了処理
	FileManager::Destroy();
	LZ77Decompresser::Destroy();

	//終了入力の受付
	cout << "圧縮が完了しました。\n任意のキーで終了します" << endl;
	getchar();

	return 0;

}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
