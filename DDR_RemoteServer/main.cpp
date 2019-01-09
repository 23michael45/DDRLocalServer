#include "../../../Shared/src/Network/MessageSerializer.h"
#include "../../../Shared/src/Network/TcpServerBase.h"
#include "../../../Shared/src/Network/TcpClientBase.h"
#include "../../../Shared/src/Utility/XmlLoader.h"
#include "../../../Shared/src/Utility/LocalizationLoader.h"
#include "../../../Shared/src/Utility/MiniDump.h"
#include "../../../Shared/src/Utility/CommonFunc.h"
#include "../../../Shared/src/Network/UdpSocketBase.h"
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


#include "../../../Shared/thirdparty/cpp-sqlite3/cppsqlite3.h"
#include <ctime>
#include <iostream>

#include <iostream>
#include <fstream>

#include "../../Shared/src/Utility/Logger.h"
#include <regex>

using namespace std;


class _ConsoleDebug : public DDRFramework::ConsoleDebug ,public CSingleton<_ConsoleDebug>
{
public:
	_ConsoleDebug()
	{
		AddCommand("ls sc", std::bind(&_ConsoleDebug::ListServerConnections, this));
	}
	void ListServerConnections()
	{
		printf_s("\nServer Connections");
		for (auto spSession : GlobalManager::Instance()->GetTcpServer()->GetLSMap())
		{


			std::string ip = spSession.second->GetSocket().remote_endpoint().address().to_string();
			printf_s("LS:\n%s  name:%i", ip.c_str(), spSession.second->GetRegisteLSInfo().name());
		}

		for (auto spSession : GlobalManager::Instance()->GetTcpServer()->GetClientMap())
		{
			std::string ip = spSession.second->GetSocket().remote_endpoint().address().to_string();
			printf_s("Client:\n%s  name:%i", ip.c_str(), spSession.second->GetRemoteLoginInfo().username());
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