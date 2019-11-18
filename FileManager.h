#include <stdio.h>
#include <string>

class FileManager
{
public:
	static FileManager *GetInstance()
	{
		return s_pInstance;
	}

	static void Create()
	{
		if (!s_pInstance)
		{
			s_pInstance = new FileManager;
		}
	}
	static void Destroy()
	{
		delete s_pInstance;
		s_pInstance = nullptr;
	}

	int LoadFile(const char* fileName, std::string* out);
	int SaveFile(const char* fileName, std::string* in);

protected:
	static FileManager *s_pInstance;
	FileManager();
};
