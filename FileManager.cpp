
#include <fstream>
#include <iostream>
#include <iterator>

#include "FileManager.h"

FileManager *FileManager::l_pInstance = nullptr;

int FileManager::LoadFile(const char* fileName, std::string* out)
{
	std::ifstream ifs(fileName);
	if (ifs.fail())
	{
		std::cerr << "読み込みエラー" << std::endl;
		return -1;
	}
	std::istreambuf_iterator<char> it(ifs);
	std::istreambuf_iterator<char> last;
	std::string str(it, last);
	*out = str;

	return 0;
	
}

int FileManager::SaveFile(const char* fileName, std::string* in)
{
	std::ofstream ofs;
	ofs.open(fileName);
	if (ofs.fail())
	{
		std::cerr << "読み込みエラー" << std::endl;
		return -1;
	}
	ofs << *in;
	ofs.close();

	return 0;
}

FileManager::FileManager()
{

}

FileManager::~FileManager()
{
	//Destroy();
}
