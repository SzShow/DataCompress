
#ifndef FILE_MANAGER_H_
#define FILE_MANAGER_H_

#include <stdio.h>
#include <string>

class FileManager
{
public:
	static FileManager* GetInstance()
	{
		return l_pInstance;
	}


	static void Create()
	{
		if (!l_pInstance)
		{
			l_pInstance = new FileManager;
		}
	}
	static void Destroy()
	{
		delete l_pInstance;
		l_pInstance = nullptr;
	}
	int LoadFile(const char* fileName, std::string* out);
	int SaveFile(const char* fileName, std::string* in);

protected:
	static FileManager *l_pInstance;
	FileManager();
	~FileManager();
};

#endif//FILE_MANAGER_H_


