#pragma once
#include <string>
std::string g_BuildTime = "4/9/2019 4:32:20 PM";
std::string g_Version = "1.0.0";

#ifdef _DEBUG
std::string g_DMode = "Debug";
#else

std::string g_DMode = "Release";
#endif
