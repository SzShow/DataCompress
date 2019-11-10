// DataCompress.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

//Source: https://gist.github.com/MajedSiefALnasr/32cff52b01d764ee158f2c36e4e84d95

#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

#define TRUE 1
#define FALSE 0

#include "FileManager.h"

typedef unsigned char byte;

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

int main(int argc, char *argv[])
{
    // 初期化
	FILE* fp_i;
	FILE* fp_o;

	FileManager::Create();
	FileManager* fManager = FileManager::GetInstance();

	int dic_bits, code_bits;

	if(argc < 5)
	{
		exit(8);
	}

	// ファイル読み込み
	
	fManager->LoadFile(argv[1], fp_i);
	

	// 圧縮の実行
	// 
	fp_o = fopen(argv[2], "wb");
	if (fp_o == NULL)
	{
		fprintf(stderr, "Output File cannot open\n");
		exit(8);
	}
	dic_bits = atoi(argv[3]);
	code_bits = atoi(argv[4]);


	// ファイルの出力


	//終了処理
	FileManager::Destroy();
	fclose(fp_i);
	fclose(fp_o);

	//終了入力の受付


}

string LZ77(string input, int option)
{
	string result;
	int length, char_info_selc = 0;

	if (option == 1)
	{
	check_char:
		length = (int)input.length();
		if (length <= 2)
		{
			cout << "enter at leaset 3 charcters \n";
			getline(cin, input);
			goto check_char;
		}

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

		int** char_info = new int* [3];
		for (int i = 0; i < length; i++)
		{
			char_info[i] = new int[length];
		}
		for (int i = 0; i < length; i++)
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
		for (int i = 1; i < length; i++)
		{
			// 
			for (int j = 0; j < i; j++)
			{
				if (input[i] == input[j])
				{
					char_info[0][char_info_selc] = i - j;
					char_info_selc++;
				}

			}

			for (int j = 0; j < length; j++)
			{
				if (char_info[0][j] != 0)
				{
					int start = i - char_info[0][j];
					int count = 1;

					for (int k = 0; k < length; k++)
					{
						if (input[start + count] == input[i + count])
						{
							count++;
						}
						else
						{
							char_info[i][j] = count;

							if (i != (length - 1))
							{
								if (char_info[0][j] + count == length)
								{
									char_info[2][j] = 0;
								}
								else
								{
									char_info[2][j] = input[char_info[0][j] + count];
								}
							
							}
							else
							{
								char_info[2][j] = NULL;
							}
						}
					}
				}


			}

			int large = 0;

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
				char_info[2][large] = input[i];
			}
			else
			{
				i += char_info[1][large];
				char_info[2][large] = input[i];
			}

			result_aray[0][result_count] = char_info[0][large];
			result_aray[1][result_count] = char_info[1][large];
			result_aray[2][result_count] = char_info[2][large];

			result_count++;

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
			if (result_aray[2][j] == 0 && result_aray[1][j] == 0)
			{
				if (result_aray[2][j] != NULL || result_aray[2][j] != 0)
				{
					char z = result_aray[2][j];
					result += to_string(result_aray[0][j]) + "," + to_string(result_aray[1][j]) + "," + z + " ";
				}
			}
		}

		return result;


	}
	else if (option == 2)
	{
		vector<string> s_input = split(input, ' ');

		for (int i = 0; i < s_input.size(); ++i)
		{
			vector<string> ss_input = split(s_input[i], ',');

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
			return result;
		}
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
