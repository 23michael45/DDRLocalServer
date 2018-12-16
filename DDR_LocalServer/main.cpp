#include "../../Shared/src/Network/MessageSerializer.h"
#include "../../Shared/src/Network/TcpServerBase.h"
#include "../../Shared/src/Network/TcpClientBase.h"
#include "LocalTcpServer.h"
#include "../../Shared/src/Utility/XmlLoader.h"
#include "../../Shared/src/Utility/MiniDump.h"
#include "../../Shared/src/Network/UdpSocketBase.h"
#include "LocalServerUdpDispatcher.h"
#include "GlobalManager.h"

#include <thread>
#include <chrono>
using namespace DDRFramework;
using namespace DDRCommProto;
using namespace std;


char gQuit = 0;


int main()
{
	InitMinDump();

	GlobalManager::Instance()->StartTcpServer();
	GlobalManager::Instance()->StartUdpServer();


	while (!gQuit)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	return 0;
}