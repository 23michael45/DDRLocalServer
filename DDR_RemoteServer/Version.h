#pragma once
#include <string>
std::string g_BuildTime = "2/14/2019 6:09:54 PM";
std::string g_Version = "1.0.0";

#ifdef _DEBUG
std::string g_DMode = "Debug";
#else

std::string g_DMode = "Release";
#endif
