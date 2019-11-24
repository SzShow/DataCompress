
#include "LZ77Compresser.h"


using namespace std;

LZ77Compresser::LZ77Compresser(const string input):
	_input(input),
	_inputLength(static_cast<int>(input.length()))
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

void LZ77Compresser::ScanMatchingChar(const int currentIndex)
{
	int char_info_selc = 0;
	
	for (int pastIndex = 0; pastIndex < currentIndex; pastIndex++)
	{
		// 現在の文字が過去の入力と一致した場合
		// 過去の入力と現在の入力の配列的距離を記録
		if (_input[currentIndex] == _input[pastIndex])
		{
			_bufferMatchingIndex[char_info_selc] = 
				currentIndex - pastIndex;
			char_info_selc++;
		}

	}
}

void LZ77Compresser::AssignValueToBuffer(int* currentIndex)
{
	// char_infoについて走査
	// CharToCode();
	for (int infoIndex = 0; infoIndex < _inputLength; infoIndex++)
	{

		if (_bufferMatchingIndex[infoIndex] != 0)
		{
			// 一致箇所の配列全体でのインデックスを代入
			int start = *currentIndex - _bufferMatchingIndex[infoIndex];
			int count = 1;

			// 一致箇所から一つずつインデックスを進める
			for (int k = 0; k < _inputLength; k++)
			{
				// 連続した一致箇所を探す
				if (_input[start + count] == _input[*currentIndex + count])
				{
					count++;
				}
				else
				{
					// 一致箇所の検出が途切れた場合

					// 長さを入力
					_bufferMatchingLength[infoIndex] = count;

					// 現在位置が入力文字列の末尾であるかチェック
					if (*currentIndex != (_inputLength - 1))
					{
						// 
						if (_bufferMatchingIndex[infoIndex] + count ==
							_inputLength)
						{
							_bufferLastChar[infoIndex] = 0;
						}
						else
						{
							// 
							_bufferLastChar[infoIndex] =
								_input[_bufferMatchingIndex[infoIndex] + count];
						}

					}
					else
					{
						_bufferLastChar[infoIndex] = NULL;
					}
				}
			}
		}

		int large = 0;
		//<int>
		//ArrayMaxGetIndex(_bufferLength);

		if (_bufferMatchingLength[large] == 0)
		{
			_bufferLastChar[large] = _input[currentIndex];
		}
		else
		{
			*currentIndex += _bufferMatchingLength[large];
			_bufferLastChar[large] = _input[currentIndex];
		}


	}
}

void LZ77Compresser::SetResultArries(const int targetIndex)
{
	_resultMatchingIndex[targetIndex] = _bufferMatchingIndex[_indexOfMaxLength];
	_resultMatchingLength[targetIndex] = _bufferMatchingLength[_indexOfMaxLength];
	_resultLastChar[targetIndex] = _bufferLastChar[_indexOfMaxLength];
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

string LZ77Compresser::ConstructCode(const int target)
{
	string out;
	if (_resultMatchingIndex[target] == 0 && _resultMatchingLength[target] == 0)
	{
		if (_resultLastChar[target] != NULL || _resultLastChar[target] != 0)
		{
			char z = _resultLastChar[target];
			out = to_string(_resultMatchingIndex[target]) + "," +
				to_string(_resultMatchingLength[target]) + "," +
				z + " ";
		}
	}
	else
	{
		char z = _resultLastChar[target];
		out = to_string(_resultMatchingIndex[target]) + "," +
			to_string(_resultMatchingLength[target]) + "," +
			z + " ";
	}

	return out;
}
string Compress(const string input)
{
	LZ77Compresser comp = LZ77Compresser(input);
	string result;
	

	int result_count = 1;

	// 入力データの走査
	// i: 符号化対象の文字のインデックス
	// j: 過去の入力文字のインデックス
	for (int currentIndex = 1; currentIndex < length; currentIndex++)
	{
		// ScanMatchingChar(currentIndex);

		// AssignValueToBuffer(currentIndex);




		// SetResultArries(result_count);

		result_count++;

		// ResetBufferValue();
	}

	string code;
	for (int j = 0; j < length; j++)
	{
		// code = ConstructCode();
		// result += code;
	}

	return result;

}
