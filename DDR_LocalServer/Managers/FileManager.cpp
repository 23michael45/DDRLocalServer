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

#include "GlobalManager.h"

#include "../../../Shared/src/Utility/CommonFunc.h"
#include "../../../Shared/src/Utility/DDRMacro.h"
#include "../../../Shared/src/Utility/Logger.h"
using namespace cppfs;

FileManager::FileManager()
{

	m_RootPath = GlobalManager::Instance()->GetLocalServerConfig().GetValue("FileCacheBaseDir");

}
FileManager::~FileManager()
{

}

std::string FileManager::HttpAddr2BaseDir(std::string httpaddr)
{
	int index = httpaddr.find_first_of(':');
	index = httpaddr.find(':',index + 1);
	index = httpaddr.find('/', index + 1);

	std::string full = httpaddr.replace(httpaddr.begin(), httpaddr.begin() + index, m_RootPath);
	full = replace_all(full, "///", "/");
	full = replace_all(full, "//", "/");

	return full;
}

std::string FileManager::GetRelativeDir(std::string httpaddr)
{
	int index = httpaddr.find_first_of(':');
	index = httpaddr.find(':', index + 1);
	index = httpaddr.find('/', index + 1);

	std::string relativepath = httpaddr.replace(httpaddr.begin(), httpaddr.begin() + index, "");
	relativepath = replace_all(relativepath, "///", "/");
	relativepath = replace_all(relativepath, "//", "/");

	return relativepath;

}

bool FileManager::FileExist(std::string url)
{
	std::string full = url;
	if (url.substr(0, 4) == "http")
	{
		full = HttpAddr2BaseDir(url);
	}

	cppfs::FileHandle fhandel = fs::open(full);
	if (fhandel.exists())
	{
		return true;
	}
	return false;
}

