#pragma once
#include <string>
std::string g_BuildTime = "3/28/2019 11:15:08 AM";
std::string g_Version = "1.0.0";

#ifdef _DEBUG
std::string g_DMode = "Debug";
#else

std::string g_DMode = "Release";
#endif
#pragma once
