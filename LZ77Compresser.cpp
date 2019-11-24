
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
		// ���݂̕������ߋ��̓��͂ƈ�v�����ꍇ
		// �ߋ��̓��͂ƌ��݂̓��͂̔z��I�������L�^
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
	// char_info�ɂ��đ���
	// CharToCode();
	for (int infoIndex = 0; infoIndex < _inputLength; infoIndex++)
	{

		if (_bufferMatchingIndex[infoIndex] != 0)
		{
			// ��v�ӏ��̔z��S�̂ł̃C���f�b�N�X����
			int start = *currentIndex - _bufferMatchingIndex[infoIndex];
			int count = 1;

			// ��v�ӏ��������C���f�b�N�X��i�߂�
			for (int k = 0; k < _inputLength; k++)
			{
				// �A��������v�ӏ���T��
				if (_input[start + count] == _input[*currentIndex + count])
				{
					count++;
				}
				else
				{
					// ��v�ӏ��̌��o���r�؂ꂽ�ꍇ

					// ���������
					_bufferMatchingLength[infoIndex] = count;

					// ���݈ʒu�����͕�����̖����ł��邩�`�F�b�N
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

	// ���̓f�[�^�̑���
	// i: �������Ώۂ̕����̃C���f�b�N�X
	// j: �ߋ��̓��͕����̃C���f�b�N�X
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
