#ifndef FileManager_h__
#define FileManager_h__

#include <vector>
#include <string>
#include "src/Utility/Singleton.h"
#include "src/Utility/FileManagerBase.h"

using namespace DDRFramework;
class FileManager :public FileManagerBase, public CSingleton<FileManager>
{
public:
	FileManager();
	~FileManager();


	std::string GetHttpServerUrl(std::string relativePath);
protected:

	std::string m_HttpFileServerUrl;
};

#endif // FileManager_h__
