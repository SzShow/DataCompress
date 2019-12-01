
#include "FileManager.h"

FileManager *FileManager::l_pInstance = nullptr;

int FileManager::LoadFile(const char* fileName, std::string* out)
{
	SaveFileName(fileName);

	std::ifstream ifs(fileName);
	if (ifs.fail())
	{
		std::cerr << "読み込みエラー" << std::endl;
		return -1;
	}
	std::istreambuf_iterator<char> it(ifs);
	std::istreambuf_iterator<char> last;
	std::string str(it, last);

	// 入力サイズ保存
	_inputSize = str.size();

	*out = str;

	return 0;
	
}

int FileManager::SaveFile(
	const std::string* fileName, const std::string* extension, std::string* in)
{
	// 出力サイズ保存, 圧縮率チェック
	_outputSize = in->length();
	if (_outputSize > _inputSize &&
		*extension == "cmp")
	{
		float rate = ((
			static_cast<float>(_outputSize) /
			static_cast<float>(_inputSize)) * 100) - 100;
		std::cerr << "サイズが" << rate << "%大きくなっています" << std::endl;
	}

	std::ofstream ofs;
	ofs.open(*fileName);
	if (ofs.fail())
	{
		std::cerr << "読み込みエラー" << std::endl;
		return -1;
	}
	ofs << *in;
	ofs.close();

	return 0;
}

bool FileManager::SaveFileName(const char* fileName)
{
	std::stringstream str(fileName);
	std::string tok;

	std::getline(str, tok, '.');
	_basicName = tok;

	std::getline(str, tok, '.');
	_extensionName = tok;
	
	return true;
}

FileManager::FileManager():
	_inputSize(0), _outputSize(0)
{

}

FileManager::~FileManager()
{
	//Destroy();
}
