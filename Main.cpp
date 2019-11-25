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

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

#define TRUE 1
#define FALSE 0

#include "FileManager.h"
#include "UtilityFunction.h"

typedef unsigned char byte;

string LZ77(string input, int option);

vector <string> split(string str, char delimiter)
{
	vector<string> internal;
	stringstream ss(str);
	string tok;

	while (getline(ss, tok, delimiter))
	{
		internal.push_back(tok);
	}

	return internal;
}

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
	LZ77Compresser lz77(input);

	int option = *argv[3] - '0';
	output = LZ77(input, option);


	// ファイルの出力
	errorCode = fManager->SaveFile(argv[2], &output);
	if (errorCode != 0)
	{
		return errorCode;
	}

	//終了処理
	FileManager::Destroy();

	//終了入力の受付
	cout << "圧縮が完了しました。\n任意のキーで終了します" << endl;
	getchar();

	return 0;

}

string LZ77(string input, int option)
{
	string result;
	int length, char_info_selc = 0;

	if (option == 1)
	{
	//check_char:
		length = (int)input.length();
		//if (length <= 2)
		//{
		//	cout << "enter at leaset 3 charcters \n";
		//	getline(cin, input);
		//	goto check_char;
		//}

		int** result_aray = new int* [3];
		for (int i = 0; i < length; i++)
		{
			result_aray[i] = new int[length];
		}
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < length; j++)
			{
				result_aray[i][j] = 0;
			}
		}

		// char_info[0][index]: 符号化対応文字列の開始地点
		// char_info[1][index]: 符号化対応文字列の長さ
		int** char_info = new int* [3];
		for (int i = 0; i < 3; i++)
		{
			char_info[i] = new int[length];
		}
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < length; j++)
			{
				char_info[i][j] = 0;
			}
		}

		result_aray[0][0] = 0;
		result_aray[1][0] = 0;
		result_aray[2][0] = input[0];

		int result_count = 1;

		// 入力データの走査
		// i: 符号化対象の文字のインデックス
		// j: 過去の入力文字のインデックス
		for (int currentIndex = 1; currentIndex < length; currentIndex++)
		{
			// 過去の入力に対して走査
			for (int pastIndex = 0; pastIndex < currentIndex; pastIndex++)
			{
				// 現在の文字が過去の入力と一致した場合
				// 過去の入力と現在の入力の配列的距離を記録
				if (input[currentIndex] == input[pastIndex])
				{
					char_info[0][char_info_selc] = currentIndex - pastIndex;
					char_info_selc++;
				}

			}

			// char_infoについて走査
			for (int infoIndex = 0; infoIndex < length; infoIndex++)
			{
				
				if (char_info[0][infoIndex] != 0)
				{
					// 一致箇所の配列全体でのインデックスを代入
					int start = currentIndex - char_info[0][infoIndex];
					int count = 1;

					// 一致箇所から一つずつインデックスを進める
					for (int k = 0; k < length; k++)
					{
						// 連続した一致箇所を探す
						if (input[start + count] == input[currentIndex + count])
						{
							count++;
						}
						else
						{
							// 一致箇所の検出が途切れた場合

							// 長さを入力
							char_info[1][infoIndex] = count;

							// 現在位置が入力文字列の末尾であるかチェック
							if (currentIndex != (length - 1))
							{
								// 
								if (char_info[0][infoIndex] + count == length)
								{
									char_info[2][infoIndex] = 0;
								}
								else
								{
									// 
									char_info[2][infoIndex] = input[char_info[0][infoIndex] + count];
								}
							
							}
							else
							{
								char_info[2][infoIndex] = NULL;
							}
						}
					}
				}


			}

			int large = 0;

			// 最も大きい長さの一致を探索
			for (int k = 1; k < length; k++)
			{
				
				if (char_info[1][large] == char_info[1][k])
				{
					large = k;
				}
				else if (char_info[1][large] < char_info[1][k])
				{
					large = k;
				}
			}

			if (char_info[1][large] == 0)
			{
				char_info[2][large] = input[currentIndex];
			}
			else
			{
				currentIndex += char_info[1][large];
				char_info[2][large] = input[currentIndex];
			}

			result_aray[0][result_count] = char_info[0][large];
			result_aray[1][result_count] = char_info[1][large];
			result_aray[2][result_count] = char_info[2][large];

			result_count++;

			// char_infoの初期化
			//
			for (int z = 0; z < 2; z++)
			{
				for (int j = 0; j < length; j++)
				{
					char_info[z][j] = 0;
				}
			}

			char_info_selc = 0;
		}

		for (int j = 0; j < length; j++)
		{
			if (result_aray[0][j] == 0 && result_aray[1][j] == 0)
			{
				if (result_aray[2][j] != NULL || result_aray[2][j] != 0)
				{
					char z = result_aray[2][j];
					result += to_string(result_aray[0][j]) + "," + to_string(result_aray[1][j]) + "," + z + " ";
				}
			}
			else
			{
				char z = result_aray[2][j];
				result += to_string(result_aray[0][j]) + "," + to_string(result_aray[1][j]) + "," + z + " ";
			}
		}

		return result;


	}
	else if (option == 2)
	{
		vector<string> s_input = split(input, ' ');

		for (unsigned int i = 0; i < s_input.size(); ++i)
		{
			if (s_input[i].size() == 0)
			{
				continue;
			}

			vector<string> ss_input = split(s_input[i], ',');

			if (s_input[i][4] == ',')
			{
				ss_input[2] = ',';
			}
			else if (ss_input.size() == 2)
			{
				ss_input.push_back(" ");
			}

			//if ((int)ss_input)

			int p = stoi(ss_input[0]), l = stoi(ss_input[1]);
			string ch;

			if (ss_input[2][0] == '0')
			{
				ch = ' ';
			}
			else
			{
				ch = ss_input[2];
			}

			if (p != 0)
			{
				int result_len = (int)result.length();
				for (int x = 0; x < l; x++)
				{
					result += result[result_len - p + x];
				}
			}

			if (ch[0] != '0' || ch[0] != NULL)
			{
				result += ch;
			}
			
		}

		return result;
	}
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
