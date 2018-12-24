#ifndef FileManager_h__
#define FileManager_h__

#include <filesystem>
#include <vector>
#include <string>
#include "../../../Shared/src/Utility/Singleton.h"

using namespace DDRFramework;

class FileManager : public CSingleton<FileManager>
{
public:	 
	FileManager();
	~FileManager();

	void SetRootPath(std::string root);
	std::vector<std::string> CheckFiles();
private:

	void CheckDir(std::string dir, std::vector<std::string>& vec);

	std::string m_RootPath;
};

#endif // FileManager_h__
