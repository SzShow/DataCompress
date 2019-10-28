
#include "FileManager.h"

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
