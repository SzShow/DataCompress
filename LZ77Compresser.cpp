
#include "LZ77Compresser.h"


using namespace std;

// コア
int LZ77Compresser::Compress(const string* extensionName, string* output)
{
	// 入力データの走査
	while (_currentIndex < _inputLength)
	{

		ScanMatchingChar();

		CountMatchingLength();
		
		// 符号化する文字列に続く文字を保存
		PickupLastChar();
		
		// 最も長い一致文字列を出力用バッファに保存
		SetResultBuffer();
		
		ResetBufferValue();
		
		// 一致文字列の長さから次の符号化対象文字列を決定
		DecideNextIndex();

	}

	// 出力への書き込み
	string code;
	*output += *extensionName + "_"; //拡張子を最初に入れる
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
	_resultCount(0),
	_largestIndex(0)
{
	_resultMatchingIndex = new unsigned char[_inputLength];
	for (int i = 0; i < _inputLength; i++)
	{
		_resultMatchingIndex[i] = 0;
	}

	_resultMatchingLength = new unsigned char[_inputLength];
	for (int i = 0; i < _inputLength; i++)
	{
		_resultMatchingLength[i] = 0;
	}

	_resultLastChar = new int[_inputLength];
	for (int i = 0; i < _inputLength; i++)
	{
		_resultLastChar[i] = 0;
	}

	_bufferMatchingIndex = new unsigned char[_inputLength];
	for (int i = 0; i < _inputLength; i++)
	{
		_bufferMatchingIndex[i] = 0;
	}

	_bufferMatchingLength = new unsigned char[_inputLength];
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
	//Destroy();
}


// 基本バッファの操作
void LZ77Compresser::ScanMatchingChar()
{
	int firstMatchCount = 0;
	int winStart = _currentIndex - UCHAR_MAX;
	if (winStart < 0)
	{
		winStart = 0;
	}
	
	for (int pastIndex = winStart; pastIndex < _currentIndex; pastIndex++)
	{
		// 現在の文字が過去の入力と一致した場合
		// 過去の入力と現在の入力の距離を記録
		if (_input[_currentIndex] == _input[pastIndex])
		{
			_bufferMatchingIndex[firstMatchCount] = 
				_currentIndex - pastIndex;
			firstMatchCount++;
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
					if (count > UCHAR_MAX)
					{
						count = UCHAR_MAX;
					}

					_bufferMatchingLength[bufferIndex] = count;

					break;
				}
			}
		}
	}
}

void LZ77Compresser::PickupLastChar()
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
						_input[_currentIndex +
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
		if (_resultLastChar[targetIndex] != 0)
		{
			char z = _resultLastChar[targetIndex];
			out.push_back(z);
			out.push_back(' ');
		}
	}
	else
	{
		char z = _resultLastChar[targetIndex];
		out.push_back(_resultMatchingIndex[targetIndex]);
		out.push_back(_resultMatchingLength[targetIndex]);
		out.push_back(z);
		out.push_back(' ');
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


// シングルトンインスタンスの初期化
LZ77Compresser* LZ77Compresser::l_pInstance = nullptr;
