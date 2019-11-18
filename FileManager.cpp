
#include "FileManager.h"
#include <iostream>
#include <iterator>

FileManager *FileManager::s_pInstance = nullptr;

static void FileManager::Create()
{
	if(!s_pInstance)
	{
		s_pInstance = new FileManager;
	}
}

static void FileManager::Destroy()
{
	delete s_pInstance;
	s_pInstance = nullptr;
}

void FileManager::LoadFile(char* fileName, string* out)
{
	std::ifstream ifs(*fileName);
	if (ifs.fail())
	{
		std::cerr << "“Ç‚Ýž‚ÝƒGƒ‰[" << std::endl;
		return -1;
	}
	std::istreambuf_iterator<char> it(ifs);
	std::istreambuf_iterator<char> last;
	std::string str(it, last);
	std::string* out = &str;
	
}
