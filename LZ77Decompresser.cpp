#include "LZ77Decompresser.h"

using namespace std;

// �R�A
int LZ77Decompresser::Decompress(string* output, string* extension)
{
	// ��Ɋg���q�̂ݒ��o
	//ExtractExtension();

	// �X�y�[�X�ɂ��ē��͂𕪊����A�e������vector�Ƃ��ĕۑ�
	SplitCompressed(extension);

	// �e�����ɂ��Đ擪���珇�Ԃɑ���
	for (unsigned int splitedIndex = 0;
		splitedIndex < _splited.size();
		++splitedIndex)
	{
		// ���݂̃C���f�b�N�X�ɑΉ����镄�������v�f�ɕ���
		SplitCompressElements(splitedIndex);

		// �f�R�[�h
		*output += Decode();
	}

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
		_splited.push_back(tok);
	}

	// �X�y�[�X���̂��̂𕜍����镔���̓t�H�[�}�b�g�����������Ȃ�̂�
	// ���̃u���b�N�ł�����C��
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
			// �x���̂Ō�ő��̎�@������
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
			result += 
				_pastOutput[resultLength - _codeIndex + matchingIndex];
		}
	}

	// ������NULL�|�C���^�܂���NULL�����łȂ���Ώo��
	if (ch[0] != '0' ||
		ch[0] != NULL)
	{
		result += ch;
	}

	_pastOutput += result;
	return result;
}


// �V���O���g���C���X�^���X�̏�����
LZ77Decompresser* LZ77Decompresser::l_pInstance = nullptr;
