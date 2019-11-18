#include <stdio.h>
#include <string>

class FileManager
{
public:
	static FileManager *GetInstance()
	{
		return s_pInstance;
	}

	static void Create();
	static void Destroy();

	void LoadFile(char* fileName, string* out);
	void SaveFile();

protected:
	static FileManager *s_pInstance;
	FileManager();
};
