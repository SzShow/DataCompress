#include "LZ77Decompresser.h"

using namespace std;

// コア
int LZ77Decompresser::Decompress(string* output, string* extension)
{
	// 先に拡張子のみ抽出
	//ExtractExtension();

	// スペースについて入力を分割し、各符号のvectorとして保存
	SplitCompressed(extension);

	// 各符号について先頭から順番に走査
	for (unsigned int splitedIndex = 0;
		splitedIndex < _splited.size();
		++splitedIndex)
	{
		// 現在のインデックスに対応する符号を諸要素に分割
		SplitCompressElements(splitedIndex);

		// デコード
		*output += Decode();
	}

	return 0;
}


// コンストラクタ・デコンストラクタ
LZ77Decompresser::LZ77Decompresser(const string input):
	_input(input),
	_codeIndex(0),
	_codeLength(0)
{

}

LZ77Decompresser::~LZ77Decompresser()
{

}


// 内部関数
void LZ77Decompresser::SplitCompressed(string* extension)
{
	stringstream ss(_input);
	string tok;

	// 先に拡張子を抽出
	getline(ss, tok, '_');
	*extension = tok;

	while (getline(ss, tok, ' '))
	{
		_splited.push_back(tok);
	}

	// スペースそのものを復号する部分はフォーマットがおかしくなるので
	// このブロックでそれを修正
	for (int splitedIndex = 0;
		splitedIndex < static_cast<int>(_splited.size());
		++splitedIndex)
	{
		if (_splited[splitedIndex].size() == 4)
		{
			_splited[splitedIndex].push_back(' ');
		}
		else if (_splited[splitedIndex].size() == 0)
		{
			// 遅いので後で他の手法を検討
			_splited.erase(_splited.begin()+splitedIndex);
		}
	}

}

void LZ77Decompresser::SplitCompressElements(const int splitedIndex)
{
	_codeIndex = static_cast<int>(_splited[splitedIndex][0]) - '0';
	_codeLength = static_cast<int>(_splited[splitedIndex][2]) - '0';
	_codeLastChar = _splited[splitedIndex][4];
}

string LZ77Decompresser::Decode()
{
	string result;

	// 末尾の文字を決定
	string ch;
	if (_codeLastChar == "0")
	{
		ch = ' ';
	}
	else
	{
		ch = _codeLastChar;
	}

	// 符号化部分があれば先に出力に追加
	if (_codeIndex != 0)
	{
		int resultLength = static_cast<int>(_pastOutput.length());
		for (int matchingIndex = 0;
			matchingIndex < _codeLength;
			matchingIndex++)
		{
			result += 
				_pastOutput[resultLength - _codeIndex + matchingIndex];
		}
	}

	// 末尾がNULLポインタまたはNULL文字でなければ出力
	if (ch[0] != '0' ||
		ch[0] != NULL)
	{
		result += ch;
	}

	_pastOutput += result;
	return result;
}


// シングルトンインスタンスの初期化
LZ77Decompresser* LZ77Decompresser::l_pInstance = nullptr;
