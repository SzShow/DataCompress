#include <stdio.h>

class FileManager
{
public:
	static FileManager *GetInstance()
	{
		return s_pInstance;
	}

	static void Create();
	static void Destroy();

	void LoadFile(char* fileName, FILE* out);
	void SaveFile();

protected:
	static FileManager *s_pInstance;
	FileManager();
};
