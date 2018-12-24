#include "FileManager.h"

#include <iostream>
#include <string>
#include <regex>
#include <cppfs/fs.h>
#include <cppfs/FileHandle.h>
#include <cppfs/FileIterator.h>
#include <cppfs/FilePath.h>

#ifdef _WINDOWS
#include <cppfs/windows/LocalFileSystem.h>
#endif

#include "../../../Shared/src/Utility/DDRMacro.h"
#include "../../../Shared/src/Utility/Logger.h"
using namespace cppfs;

FileManager::FileManager()
{

	FilePath path(cppfs::getexepath());
	m_RootPath = path.directoryPath();
	

}
FileManager::~FileManager()
{

}

void FileManager::SetRootPath(std::string root)
{
	m_RootPath = root;
}


void FileManager::CheckDir(std::string dir, std::vector<std::string>& vec)
{
	FileHandle fhandel = fs::open(dir);
	if (fhandel.exists())
	{
		if (fhandel.isDirectory())
		{
			auto files = fhandel.listFiles();
			



			for (auto file : files)
			{
				CheckDir(dir + "/" + file, vec);
			}
		}
		else
		{
			vec.push_back(dir);
		}

	}
}
std::vector<string> FileManager::CheckFiles()
{
	std::vector<std::string> files;
	CheckDir(m_RootPath, files);
	return files;

}
