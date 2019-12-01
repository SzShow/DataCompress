#include "LZ77Decompresser.h"

using namespace std;

// �R�A
int LZ77Decompresser::Decompress(string* output, string* extension)
{
	int errorCode = 0;

	// �X�y�[�X�ɂ��ē��͂𕪊����A�e������vector�Ƃ��ĕۑ�
	SplitCompressed(extension);

	// �e�����ɂ��Đ擪���珇�Ԃɑ���
	for (unsigned int splitedIndex = 0;
		splitedIndex < _codes.size();
		++splitedIndex)
	{
		// ���݂̃C���f�b�N�X�ɑΉ����镄�������v�f�ɕ���
		errorCode = SplitCompressElements(splitedIndex);
		if (errorCode != 0)
		{
			return errorCode;
		}

		// �f�R�[�h
		Decode();
	}

	*output = _pastOutput;

	return 0;
}


// �R���X�g���N�^�E�f�R���X�g���N�^
LZ77Decompresser::LZ77Decompresser(const string input):
	_input(input),
	_codeIndex(0),
	_codeLength(0)
{

}

LZ77Decompresser::~LZ77Decompresser()
{

}


// �����֐�
void LZ77Decompresser::SplitCompressed(string* extension)
{
	stringstream ss(_input);
	string tok;

	// ��Ɋg���q�𒊏o
	getline(ss, tok, '_');
	*extension = tok;

	while (getline(ss, tok, ' '))
	{
		_codes.push_back(tok);
	}

	// �X�y�[�X���̂��̂𕜍����镔���̓t�H�[�}�b�g�����������Ȃ�̂�
	// ���̃u���b�N�ł�����C��
	for (int splitedIndex = 0;
		splitedIndex < static_cast<int>(_codes.size());
		++splitedIndex)
	{
		if (_codes[splitedIndex].size() == 0)
		{
			_codes[splitedIndex].push_back(' ');

			// �x���̂Ō�ő��̎�@������
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
		cerr << "�z�肳��ĂȂ������T�C�Y�����o����܂���" << endl;
		return 1;
	}

	return 0;

}

void LZ77Decompresser::Decode()
{

	// �����̕���������
	string ch;
	if (_codeLastChar == "0")
	{
		ch = ' ';
	}
	else
	{
		ch = _codeLastChar;
	}

	// ����������������ΐ�ɏo�͂ɒǉ�
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

	// ������NULL�|�C���^�܂���NULL�����łȂ���Ώo��
	if (ch[0] != '0' ||
		ch[0] != NULL)
	{
		_pastOutput += ch;
	}
}


// �V���O���g���C���X�^���X�̏�����
LZ77Decompresser* LZ77Decompresser::l_pInstance = nullptr;
