#include "LZ77Decompresser.h"

using namespace std;

// コア
int LZ77Decompresser::Decompress(string* output, string* extension)
{
	int errorCode = 0;

	// スペースについて入力を分割し、各符号のvectorとして保存
	SplitCompressed(extension);

	// 各符号について先頭から順番に走査
	for (unsigned int splitedIndex = 0;
		splitedIndex < _codes.size();
		++splitedIndex)
	{
		// 現在のインデックスに対応する符号を諸要素に分割
		errorCode = SplitCompressElements(splitedIndex);
		if (errorCode != 0)
		{
			return errorCode;
		}

		// デコード
		Decode();
	}

	*output = _pastOutput;

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
		_codes.push_back(tok);
	}

	// スペースそのものを復号する部分はフォーマットがおかしくなるので
	// このブロックでそれを修正
	for (int splitedIndex = 0;
		splitedIndex < static_cast<int>(_codes.size());
		++splitedIndex)
	{
		if (_codes[splitedIndex].size() == 0)
		{
			_codes[splitedIndex].push_back(' ');

			// 遅いので後で他の手法を検討
			_codes.erase(_codes.begin() + splitedIndex + 1);
		}
	}

}

int LZ77Decompresser::SplitCompressElements(const int splitedIndex)
{
	if (_codes[splitedIndex].size() == 1)
	{
		_codeIndex = 0;
		_codeLength = 0;
		_codeLastChar = _codes[splitedIndex][0];
	}
	else if(_codes[splitedIndex].size() == 3)
	{
		_codeIndex = static_cast<unsigned char>(_codes[splitedIndex][0]);
		_codeLength = static_cast<unsigned char>(_codes[splitedIndex][1]);
		_codeLastChar = _codes[splitedIndex][2];
	}
	else
	{
		cerr << "想定されてない符号サイズが検出されました" << endl;
		return 1;
	}

	return 0;

}

void LZ77Decompresser::Decode()
{

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
			_pastOutput += 
				_pastOutput[resultLength - _codeIndex + matchingIndex];
		}
	}

	// 末尾がNULLポインタまたはNULL文字でなければ出力
	if (ch[0] != '0' ||
		ch[0] != NULL)
	{
		_pastOutput += ch;
	}
}


// シングルトンインスタンスの初期化
LZ77Decompresser* LZ77Decompresser::l_pInstance = nullptr;
