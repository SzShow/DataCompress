
//---------------------------------------------------------------------------
// Developed by Shota Suzuki
//---------------------------------------------------------------------------
/**
* @file LZ77Decompresser.h
* @brief �t�@�C�����k
* @note singleton�f�U�C���p�^�[���ɏ]���ċL�q�B
*
* �@�ڍא���
*
* LZ77�����ɂ���ē��͂��ꂽ�t�@�C�������k����N���X�ł�
* ��v������̊J�n�ʒu(index)�A��v������̒���(length)�Ȃǂ̕������ɕK�v�ȏ����W�߂����
* [index length ���������̂�����ɑ�������]���\�����3Byte�̕������o�͂��܂��B
* �܂��A��v�����񂪌�������Ȃ������ɂ��Ă͕��������܂���B
*
* SlidingWindow�̈ʒu�ƒ����ɂ��ẮA���ꂼ��ő��UCHAR_MAX�܂ł̒l�����܂��B
*
* singleton�C���X�^���X������Create()
* singleton�C���X�^���X�Ăяo����l_pInstance
* singletpn�C���X�^���X������Destroy()
*/
//---------------------------------------------------------------------------

#ifndef LZ77_COMPRESSER_H_
#define LZ77_COMPRESSER_H_

#include <string>


class LZ77Compresser
{
public:

	// ���k���s�̃C���^�[�t�F�C�X
	int Compress(const std::string* extensionName, std::string* output);

	// �C���X�^���X�̎擾
	static LZ77Compresser* GetInstance()
	{
		return l_pInstance;
	}

	// �C���X�^���X�̑���
	static void Create(const std::string input)
	{
		if (!l_pInstance)
		{
			l_pInstance = new LZ77Compresser(input);
		}
	}
	static void Destroy()
	{

		delete[] l_pInstance->_resultMatchingIndex;
		delete[] l_pInstance->_resultMatchingLength;
		delete[] l_pInstance->_resultLastChar;
		delete[] l_pInstance->_bufferMatchingIndex;
		delete[] l_pInstance->_bufferMatchingLength;
		delete[] l_pInstance->_bufferLastChar;

		l_pInstance->_input.clear();
		l_pInstance->_input.shrink_to_fit();

		delete l_pInstance;
		l_pInstance = nullptr;
	}

protected:
	// �R���X�g���N�^�E�f�R���X�g���N�^
	LZ77Compresser(const std::string input);
	~LZ77Compresser();

	
	// �����@�\
	void ScanMatchingChar();
	void CountMatchingLength();
	void PickupLastChar();
	void SetResultBuffer();
	void ResetBufferValue();
	inline void DecideNextIndex();
	int SearchLongestIndex();
	std::string Encode(const int targetIndex);

protected:
	// �V���O���g���C���X�^���X
	static LZ77Compresser* l_pInstance;

	// �����ϐ�
	std::string _input; // ���̓f�[�^
	int _inputLength; // ���̓f�[�^�̗v�f��
	int _currentIndex; // �������̑ΏۂƂȂ��Ă��镶����̍Ō��
	int _largestIndex; // �ł�������v������̊J�n�ʒu
	int _resultCount; // �Ō�ɏ������񂾏o�͗p�o�b�t�@�̃C���f�b�N�X

	// �o�̓o�b�t�@
	unsigned char* _resultMatchingIndex;
	unsigned char* _resultMatchingLength;
	int* _resultLastChar;

	// ��{�o�b�t�@
	unsigned char* _bufferMatchingIndex;
	unsigned char* _bufferMatchingLength;
	int* _bufferLastChar;
};


#endif
