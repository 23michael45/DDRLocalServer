#ifndef FileManager_h__
#define FileManager_h__

#include <filesystem>
#include <vector>
#include <string>
#include "../../../Shared/src/Utility/Singleton.h"
#include "../../../Shared/src/Utility/FileManagerBase.h"

using namespace DDRFramework;
class FileManager :public FileManagerBase, public CSingleton<FileManager>
{
public:
	FileManager();
	~FileManager();

	std::string HttpAddr2BaseDir(std::string httpaddr);
	std::string GetRelativeDir(std::string httpaddr);

	bool FileExist(std::string url);
};

#endif // FileManager_h__
