
#include "LZ77Compresser.h"


using namespace std;

int LZ77Compresser::Compress(string* output)
{

	int result_count = 1;

	// ���̓f�[�^�̑���
	while (_currentIndex < _inputLength)
	{

		// 1. �ߋ��̕����񂩂猻�݂̕����ƈ�v������̂����X�g�A�b�v
		ScanMatchingChar();

		// 2. ���X�g�A�b�v������������C���f�b�N�X���v�Z
		CountMatchingLength();
		
		// 3. ���������镶����ɑ���������ۑ�
		PickupLastChar();
		
		// 4. �ł�������v��������o�͗p�o�b�t�@�ɕۑ�
		SetResultBuffer();
		
		// 5. �ʏ�o�b�t�@�̒��g�����Z�b�g
		ResetBufferValue();
		
		// 6. ��v������̒������玟�̕������Ώە����������
		DecideNextIndex();

	}

	// �o�͂ւ̏�������
	string code;
	for (int outputIndex = 0; outputIndex < _inputLength; outputIndex++)
	{

		*output += Encode(outputIndex);

	}

	return 0;

}

// �R���X�g���N�^�E�f�R���X�g���N�^
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
	// �����ϐ�
	_input; // ���̓f�[�^

	// �o�̓o�b�t�@
	delete[] _resultMatchingIndex;
	delete[] _resultMatchingLength;
	delete[] _resultLastChar;

	// ��{�o�b�t�@
	delete[] _bufferMatchingIndex;
	delete[] _bufferMatchingLength;
	delete[] _bufferLastChar;
}

// ��{�o�b�t�@�̑���
void LZ77Compresser::ScanMatchingChar()
{
	int char_info_selc = 0;
	
	for (int pastIndex = 0; pastIndex < _currentIndex; pastIndex++)
	{
		// ���݂̕������ߋ��̓��͂ƈ�v�����ꍇ
		// �ߋ��̓��͂ƌ��݂̓��͂̔z��I�������L�^
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
		// �ꕶ���ڂ���v�����C���f�b�N�X�̂ݐ�����
		if (_bufferMatchingIndex[bufferIndex] != 0)
		{
			int start = _currentIndex - _bufferMatchingIndex[bufferIndex];
			int count = 1;

			// �����̒T���J�n
			for (int k = 0; k < _inputLength; k++)
			{
				// �A��������v�ӏ���T��
				if (_input[start + count] == _input[_currentIndex + count])
				{
					count++;
				}
				else
				{
					// ��v������̒��������
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
			// ���݈ʒu�����͕�����̖����ł��邩�`�F�b�N
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


// �o�͗p�o�b�t�@�̑���
void LZ77Compresser::SetResultBuffer()
{
	_resultCount++;
	int longestIndex = SearchLongestIndex();
	_resultMatchingIndex[_resultCount] = _bufferMatchingIndex[longestIndex];
	_resultMatchingLength[_resultCount] = _bufferMatchingLength[longestIndex];
	_resultLastChar[_resultCount] = _bufferLastChar[longestIndex];
}


// �������̎��s
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
		// �R�[�h�������Ȃ錴��������Ă���
		char z = _resultLastChar[targetIndex];
		out = to_string(_resultMatchingIndex[targetIndex]) + "," +
			to_string(_resultMatchingLength[targetIndex]) + "," +
			z + " ";
	}

	return out;
}


// ���[�e�B���e�B
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
