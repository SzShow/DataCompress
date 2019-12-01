
//---------------------------------------------------------------------------
// Developed by Shota Suzuki
//---------------------------------------------------------------------------
/**
* @file LZ77Decompresser.h
* @brief �t�@�C����
* @todo �𓀂̍ۂɃG���[�̏C��
* @note singleton�f�U�C���p�^�[���ɏ]���ċL�q�B
*
* �@�ڍא���
*
* LZ77�����ɂ���ē��͂��ꂽ�t�@�C�����𓀂���N���X�ł��B
* .cmp�t�@�C�����̐擪����g���q�𒊏o������A
* �t�@�C�����ɂ��镄����S�ēǂݎ��A
* �����𒊏o���ꂽ���Ԃ��畜�����ĉ𓀂���d�g�݂ƂȂ��Ă��܂��B
* 
*
* singleton�C���X�^���X������Create()
* singleton�C���X�^���X�Ăяo����l_pInstance
* singletpn�C���X�^���X������Destroy()
*/
//---------------------------------------------------------------------------

#ifndef LZ77_DECOMPRESSER_H_
#define LZ77_DECOMPRESSER_H_

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
//#include <memory>


class LZ77Decompresser
{
public:

	// ���k���s�̃C���^�[�t�F�C�X
	int Decompress(std::string* output, std::string* extension);


	// �C���X�^���X�̎擾
	static LZ77Decompresser* GetInstance()
	{
		return l_pInstance;
	}


	// �C���X�^���X�̑���
	static void Create(const std::string input)
	{
		if (!l_pInstance)
		{
			l_pInstance = new LZ77Decompresser(input);
		}
	}
	static void Destroy()
	{
		l_pInstance->_input.clear();
		l_pInstance->_input.shrink_to_fit();
		l_pInstance->_codes.clear();
		l_pInstance->_codes.shrink_to_fit();
		l_pInstance->_codeLastChar.clear();
		l_pInstance->_codeLastChar.shrink_to_fit();
		l_pInstance->_pastOutput.clear();
		l_pInstance->_pastOutput.shrink_to_fit();

		delete l_pInstance;
		l_pInstance = nullptr;
	}

protected:
	// �R���X�g���N�^�E�f�R���X�g���N�^
	LZ77Decompresser(const std::string input);
	~LZ77Decompresser();


	// �����@�\
	//void ExtractExtension();
	void SplitCompressed(std::string* extension);
	int SplitCompressElements(const int splitedIndex);
	void Decode();


protected:
	// �V���O���g���C���X�^���X
	static LZ77Decompresser* l_pInstance;

	// �����ϐ�
	std::string _input; // ���̓f�[�^
	std::vector<std::string> _codes;
	int _codeIndex;
	int _codeLength;
	std::string _codeLastChar;
	std::string _pastOutput;

};


#endif
