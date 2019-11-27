
#ifndef LZ77_DECOMPRESSER_H_
#define LZ77_DECOMPRESSER_H_

#include <string>
#include <vector>
#include <sstream>


class LZ77Decompresser
{
public:

	// ���k���s�̃C���^�[�t�F�C�X
	int Decompress(std::string* output);

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

		delete l_pInstance;
		l_pInstance = nullptr;
	}

protected:
	// �R���X�g���N�^�E�f�R���X�g���N�^
	LZ77Decompresser(const std::string input);
	~LZ77Decompresser();


	// �����@�\
	void SplitCompressed();
	void SplitCompressElements(const int splitedIndex);
	std::string Decode();


protected:
	// �V���O���g���C���X�^���X
	static LZ77Decompresser* l_pInstance;

	// �����ϐ�
	std::string _input; // ���̓f�[�^
	std::vector<std::string> _splited;
	int _codeIndex;
	int _codeLength;
	std::string _codeLastChar;
	std::string _pastOutput;

};


#endif
