//---------------------------------------------------------------------------
// Developed by Shota Suzuki
//---------------------------------------------------------------------------
/**
* @file Main.cpp
* @brief 動作の基点部分
* @todo stringのメモリ開放, 圧縮ファイルの圧縮ブロックの追加, あらゆるバイナリファイルへの対応
* @note 
*
* 　詳細説明
*
* LZ77方式によって入力されたファイルを圧縮するプログラムです。
* コマンドライン引数に入力ファイル名を入れると、
* 対応するファイルの圧縮・解凍ファイルが出力されます
* 圧縮ファイルには独自の形式として.cmpを用いてます。
* 
* 現在、.txtファイルの圧縮・解凍の成功までは確認しましたが、
* 画像などのバイナリファイルにおいて、解凍されたファイルが壊れてしまいます。
* この要因が何によって生じているかの検討はついており、
* 詳細はFileManager.hにて書いております。
*
*/
//---------------------------------------------------------------------------

using namespace std;

#include "FileManager.h"
#include "LZ77Compresser.h"
#include "LZ77Decompresser.h"


// argv[0]: 入力ファイル名
int main(int argc, char *argv[])
{

	int errorCode = 0;
	string input;
	string output;


	FileManager::Create();
	FileManager* fManager = FileManager::GetInstance();
	errorCode = fManager->LoadFile(argv[1], &input);
	if (errorCode != 0)
	{
		return errorCode;
	}
	cout << "ファイルの読み込みが完了しました" << endl;

	
	string outputExtension;
	string extensionName = fManager->GetExtensionName();
	if (extensionName == "cmp")
	{
		LZ77Decompresser::Create(input);
		LZ77Decompresser* lz77 = LZ77Decompresser::GetInstance();
		errorCode = lz77->Decompress(&output, &outputExtension);
		if (errorCode != 0)
		{
			return errorCode;
		}
		LZ77Decompresser::Destroy();
		cout << "解凍が完了しました。" << endl;
	}
	else
	{
		outputExtension = "cmp";
		LZ77Compresser::Create(input);
		LZ77Compresser* lz77 = LZ77Compresser::GetInstance();
		errorCode = lz77->Compress(&extensionName, &output);
		if (errorCode != 0)
		{
			return errorCode;
		}
		LZ77Compresser::Destroy();

		cout << "圧縮が完了しました" << endl;
	}

	
	string outputBaseName = fManager->GetBasicName();
	string outputName = outputBaseName + "." + outputExtension;
	errorCode = fManager->SaveFile(&outputName, &outputExtension ,&output);
	if (errorCode != 0)
	{
		return errorCode;
	}
	cout << "ファイルの保存が完了しました" << endl;
	FileManager::Destroy();


	// shrink_to_fit()を使ってもcapacityが減少しない？
	/*input.clear();
	input.shrink_to_fit();*/
	string().swap(input); //こっちでも同様
	output.clear();
	output.shrink_to_fit();
	outputExtension.clear();
	outputExtension.shrink_to_fit();
	extensionName.clear();
	extensionName.shrink_to_fit();
	outputBaseName.clear();
	outputBaseName.shrink_to_fit();
	outputName.clear();
	outputName.shrink_to_fit();
	

	cout << "任意のキーで終了します" << endl;
	system("pause");

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
