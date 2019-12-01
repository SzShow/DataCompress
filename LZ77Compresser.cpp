
#include "LZ77Compresser.h"


using namespace std;

// �R�A
int LZ77Compresser::Compress(const string* extensionName, string* output)
{
	// ���̓f�[�^�̑���
	while (_currentIndex < _inputLength)
	{

		ScanMatchingChar();

		CountMatchingLength();
		
		// ���������镶����ɑ���������ۑ�
		PickupLastChar();
		
		// �ł�������v��������o�͗p�o�b�t�@�ɕۑ�
		SetResultBuffer();
		
		ResetBufferValue();
		
		// ��v������̒������玟�̕������Ώە����������
		DecideNextIndex();

	}

	// �o�͂ւ̏�������
	string code;
	*output += *extensionName + "_"; //�g���q���ŏ��ɓ����
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
	// �����ϐ�
	//Destroy();
}


// ��{�o�b�t�@�̑���
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
		// ���݂̕������ߋ��̓��͂ƈ�v�����ꍇ
		// �ߋ��̓��͂ƌ��݂̓��͂̋������L�^
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


// �V���O���g���C���X�^���X�̏�����
LZ77Compresser* LZ77Compresser::l_pInstance = nullptr;
