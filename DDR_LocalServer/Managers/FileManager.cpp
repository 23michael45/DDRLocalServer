#include "FileManager.h"

#include <iostream>
#include <string>
#include <regex>
#include <cppfs/fs.h>
#include <cppfs/FileHandle.h>
#include <cppfs/FileIterator.h>

#include "../../../Shared/src/Utility/DDRMacro.h"
#include "../../../Shared/src/Utility/Logger.h"
using namespace cppfs;
FileManager::FileManager()
{

}
FileManager::~FileManager()
{

}

void FileManager::SetRootPath(std::string root)
{
	m_RootPath = root;
}


void FileManager::CheckFiles()
{
	//std::filesystem f;// rootPath = (m_RootPath);
	//for (auto& p : fs::directory_iterator("sandbox"))
	//	std::cout << p.path() << '\n';

	std::vector<std::string> files;

	FileHandle dir = fs::open(m_RootPath);

	if (dir.isDirectory())
	{
		for (FileIterator it = dir.begin(); it != dir.end(); ++it)
		{
			std::string path = *it;
			files.push_back(path);
		}
	}

	for (auto f : files)
	{
		DebugLog("\n%s", f.c_str());
	}

}
