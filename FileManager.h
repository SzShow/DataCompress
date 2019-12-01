
#ifndef FILE_MANAGER_H_
#define FILE_MANAGER_H_

#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>

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
		l_pInstance->_basicName.clear();
		l_pInstance->_basicName.shrink_to_fit();
		l_pInstance->_extensionName.clear();
		l_pInstance->_extensionName.shrink_to_fit();

		delete l_pInstance;
		l_pInstance = nullptr;
	}
	int LoadFile(const char* fileName, std::string* out);
	int SaveFile(const std::string* fileName, const std::string* extension, std::string* in);

	std::string GetBasicName()
	{
		return _basicName;
	}
	std::string GetExtensionName()
	{
		return _extensionName;
	}


protected:
	FileManager();
	~FileManager();

	bool SaveFileName(const char* fileName);

protected:
	static FileManager* l_pInstance;

	size_t _inputSize;
	size_t _outputSize;

	std::string _basicName;
	std::string _extensionName;

};

#endif//FILE_MANAGER_H_


