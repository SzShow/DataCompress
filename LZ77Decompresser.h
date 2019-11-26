
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

		// �o�̓o�b�t�@
		delete[] l_pInstance->_resultMatchingIndex;
		delete[] l_pInstance->_resultMatchingLength;
		delete[] l_pInstance->_resultLastChar;

		// ��{�o�b�t�@
		delete[] l_pInstance->_bufferMatchingIndex;
		delete[] l_pInstance->_bufferMatchingLength;
		delete[] l_pInstance->_bufferLastChar;

		delete l_pInstance;
		l_pInstance = nullptr;
	}

protected:
	// �R���X�g���N�^�E�f�R���X�g���N�^
	LZ77Decompresser(const std::string input);
	~LZ77Decompresser();


	// �����@�\
	void SplitCompressed();
	void CountMatchingLength();
	void PickupLastChar();
	void SetResultBuffer();
	void ResetBufferValue();
	inline void DecideNextIndex();
	std::string Encode(const int targetIndex);
	int SearchLongestIndex();

protected:
	// �V���O���g���C���X�^���X
	static LZ77Decompresser* l_pInstance;

	// �����ϐ�
	std::string _input; // ���̓f�[�^
	std::vector<std::string> _splited;
	int _inputLength; // ���̓f�[�^�̗v�f��
	int _currentIndex; // �������̑ΏۂƂȂ��Ă��镶����̍Ō��
	int _largestIndex; // �ł�������v������̊J�n�ʒu
	int _resultCount; // �Ō�ɏ������񂾏o�͗p�o�b�t�@�̃C���f�b�N�X

	// �o�̓o�b�t�@
	int* _resultMatchingIndex;
	int* _resultMatchingLength;
	int* _resultLastChar;

	// ��{�o�b�t�@
	int* _bufferMatchingIndex;
	int* _bufferMatchingLength;
	int* _bufferLastChar;
};


#endif
