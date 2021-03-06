#include "src/Network/MessageSerializer.h"
#include "src/Network/TcpServerBase.h"
#include "src/Network/TcpClientBase.h"
#include "Servers/BroadcastServerTcpServer.h"
#include "src/Utility/XmlLoader.h"
#include "src/Utility/LocalizationLoader.h"
#include "src/Utility/MiniDump.h"
#include "src/Utility/CommonFunc.h"
#include "src/Network/UdpSocketBase.h"
#include "Managers/GlobalManager.h"


#include <thread>
#include <chrono>
using namespace DDRFramework;
using namespace DDRCommProto;
using namespace std;

#ifdef _WINDOWS
#include <Windows.h>
#include <cppfs/windows/LocalFileSystem.h>
#endif
#include <cppfs/FilePath.h>


#include "thirdparty/cpp-sqlite3/cppsqlite3.h"
#include <ctime>
#include <iostream>

#include <iostream>
#include <fstream>

#include "src/Utility/Logger.h"
#include <regex>
#include "Version.h"

using namespace std;


class _ConsoleDebug : public DDRFramework::ConsoleDebug ,public CSingleton<_ConsoleDebug>
{
public:
	_ConsoleDebug()
	{
		AddCommand("ls sc", std::bind(&_ConsoleDebug::ListServerConnections, this));
		AddCommand("ver", std::bind(&_ConsoleDebug::PrintVersion, this));
	}
	void PrintVersion()
	{
		printf_s("\nBuild Time:%s", g_BuildTime.c_str());
		printf_s("\nBuild Version:%s", g_Version.c_str());
		printf_s("\nBuild Mode:%s", g_DMode.c_str());

	}
	void ListServerConnections()
	{
		printf_s("\nServer Connections");
		for (auto pair : GlobalManager::Instance()->GetTcpServer()->GetTcpSocketContainerMap())
		{


			std::string ip = pair.second->GetSocket().remote_endpoint().address().to_string();
			printf_s("\n%s", ip.c_str());
		}
	}
};


int main()
{
	DisableMouseSelectConsole();

	InitMinDump();
	GlobalManager::Instance()->StartTcpServer();
	_ConsoleDebug::Instance()->ConsoleDebugLoop();

	return 0;
}