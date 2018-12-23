#ifndef FileManager_h__
#define FileManager_h__

#include <filesystem>

#include "../../../Shared/src/Utility/Singleton.h"

using namespace DDRFramework;

class FileManager : public CSingleton<FileManager>
{
public:	 
	FileManager();
	~FileManager();

	void SetRootPath(std::string root);
	void CheckFiles();
private:
	std::string m_RootPath;
};

#endif // FileManager_h__
