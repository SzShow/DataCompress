
#include "LZ77Compresser.h"


using namespace std;

int LZ77Compresser::Compress(string* output)
{

	int result_count = 1;

	// 入力データの走査
	while (_currentIndex < _inputLength)
	{

		// 1. 過去の文字列から現在の文字と一致するものをリストアップ
		ScanMatchingChar();

		// 2. リストアップした文字からインデックスを計算
		CountMatchingLength();
		
		// 3. 符号化する文字列に続く文字を保存
		PickupLastChar();
		
		// 4. 最も長い一致文字列を出力用バッファに保存
		SetResultBuffer();
		
		// 5. 通常バッファの中身をリセット
		ResetBufferValue();
		
		// 6. 一致文字列の長さから次の符号化対象文字列を決定
		DecideNextIndex();

	}

	// 出力への書き込み
	string code;
	for (int outputIndex = 0; outputIndex < _inputLength; outputIndex++)
	{

		*output += Encode(outputIndex);

	}

	return 0;

}

// コンストラクタ・デコンストラクタ
LZ77Compresser::LZ77Compresser(const string input) :
	_input(input),
	_inputLength(static_cast<int>(input.length())),
	_currentIndex(1),
	_resultCount(0)
{
	_resultMatchingIndex = new int[_inputLength];
	for (int i = 0; i < _inputLength; i++)
	{
		_resultMatchingIndex[i] = 0;
	}

	_resultMatchingLength = new int[_inputLength];
	for (int i = 0; i < _inputLength; i++)
	{
		_resultMatchingLength[i] = 0;
	}

	_resultLastChar = new int[_inputLength];
	for (int i = 0; i < _inputLength; i++)
	{
		_resultLastChar[i] = 0;
	}

	_bufferMatchingIndex = new int[_inputLength];
	for (int i = 0; i < _inputLength; i++)
	{
		_bufferMatchingIndex[i] = 0;
	}

	_bufferMatchingLength = new int[_inputLength];
	for (int i = 0; i < _inputLength; i++)
	{
		_bufferMatchingLength[i] = 0;
	}

	_bufferLastChar = new int[_inputLength];
	for (int i = 0; i < _inputLength; i++)
	{
		_bufferLastChar[i] = 0;
	}

	_resultLastChar[0] = _input[0];
}

LZ77Compresser::~LZ77Compresser()
{
	// 内部変数
	_input; // 入力データ

	// 出力バッファ
	delete[] _resultMatchingIndex;
	delete[] _resultMatchingLength;
	delete[] _resultLastChar;

	// 基本バッファ
	delete[] _bufferMatchingIndex;
	delete[] _bufferMatchingLength;
	delete[] _bufferLastChar;
}

// 基本バッファの操作
void LZ77Compresser::ScanMatchingChar()
{
	int char_info_selc = 0;
	
	for (int pastIndex = 0; pastIndex < _currentIndex; pastIndex++)
	{
		// 現在の文字が過去の入力と一致した場合
		// 過去の入力と現在の入力の配列的距離を記録
		// 
		if (_input[_currentIndex] == _input[pastIndex])
		{
			_bufferMatchingIndex[char_info_selc] = 
				_currentIndex - pastIndex;
			char_info_selc++;
		}

	}
}

void LZ77Compresser::CountMatchingLength()
{

	for (int bufferIndex = 0; bufferIndex < _inputLength; bufferIndex++)
	{
		// 一文字目が一致したインデックスのみ数える
		if (_bufferMatchingIndex[bufferIndex] != 0)
		{
			int start = _currentIndex - _bufferMatchingIndex[bufferIndex];
			int count = 1;

			// 長さの探索開始
			for (int k = 0; k < _inputLength; k++)
			{
				// 連続した一致箇所を探す
				if (_input[start + count] == _input[_currentIndex + count])
				{
					count++;
				}
				else
				{
					// 一致文字列の長さを入力
					_bufferMatchingLength[bufferIndex] = count;

					continue;
				}
			}
		}
	}
}

int LZ77Compresser::PickupLastChar()
{
	for (int bufferIndex = 0; bufferIndex < _inputLength; bufferIndex++)
	{
		if (_bufferMatchingIndex[bufferIndex] != 0)
		{
			// 現在位置が入力文字列の末尾であるかチェック
			if (_currentIndex != (_inputLength - 1))
			{
				if (_bufferMatchingIndex[bufferIndex] +
					_bufferMatchingLength[bufferIndex] ==
					_inputLength)
				{
					_bufferLastChar[bufferIndex] = 0;
				}
				else
				{
					// 
					_bufferLastChar[bufferIndex] =
						_input[_bufferMatchingIndex[bufferIndex] +
						_bufferMatchingLength[bufferIndex]];
				}

			}
			else
			{
				_bufferLastChar[bufferIndex] = NULL;
			}
		}
		else
		{
			_bufferLastChar[bufferIndex] = _input[_currentIndex];
		}

	}

}

void LZ77Compresser::ResetBufferValue()
{
	for (int i = 0; i < _inputLength; i++)
	{
		_bufferMatchingIndex[i] = 0;
		_bufferMatchingLength[i] = 0;
		_bufferLastChar[i] = 0;
	}
}


// 出力用バッファの操作
void LZ77Compresser::SetResultBuffer()
{
	_resultCount++;
	int longestIndex = SearchLongestIndex();
	_resultMatchingIndex[_resultCount] = _bufferMatchingIndex[longestIndex];
	_resultMatchingLength[_resultCount] = _bufferMatchingLength[longestIndex];
	_resultLastChar[_resultCount] = _bufferLastChar[longestIndex];
}


// 符号化の実行
string LZ77Compresser::Encode(const int targetIndex)
{
	string out;
	if (_resultMatchingIndex[targetIndex] == 0 &&
		_resultMatchingLength[targetIndex] == 0)
	{
		if (_resultLastChar[targetIndex] != NULL ||
			_resultLastChar[targetIndex] != 0)
		{
			char z = _resultLastChar[targetIndex];
			out = to_string(_resultMatchingIndex[targetIndex]) + "," +
				to_string(_resultMatchingLength[targetIndex]) + "," +
				z + " ";
		}
	}
	else
	{
		// コードが長くなる原因を作っている
		char z = _resultLastChar[targetIndex];
		out = to_string(_resultMatchingIndex[targetIndex]) + "," +
			to_string(_resultMatchingLength[targetIndex]) + "," +
			z + " ";
	}

	return out;
}


// ユーティリティ
inline void LZ77Compresser::DecideNextIndex()
{
	_currentIndex += _resultMatchingLength[_resultCount] + 1;
}

int LZ77Compresser::SearchLongestIndex()
{
	int longestIndex = 0;
	for (int searchingIndex = 1; searchingIndex < _inputLength; searchingIndex++)
	{

		if (_bufferMatchingLength[longestIndex] ==
			_bufferMatchingLength[searchingIndex])
		{
			longestIndex = searchingIndex;
		}
		else if (_bufferMatchingLength[longestIndex] < 
			_bufferMatchingLength[searchingIndex])
		{
			longestIndex = searchingIndex;
		}
	}

	return longestIndex;
}
