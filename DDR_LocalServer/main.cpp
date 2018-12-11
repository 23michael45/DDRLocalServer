#include "../../Shared/src/Network/MessageSerializer.h"
#include "../../Shared/src/Network/TcpServerBase.h"
#include "../../Shared/src/Network/TcpClientBase.h"
#include "LocalTcpServer.h"
#include "../../Shared/src/Utility/XmlLoader.h"

#include <thread>
#include <chrono>
using namespace DDRFramework;
using namespace std;
int main()
{
	XmlLoader loader("Config/LocalServerConfig.xml");
	std::string port = loader.GetValue("Port");
	std::string servername = loader.GetValue("ServerName");
	std::string threadCount = loader.GetValue("ThreadCount");

	loader.SetValue(std::string("ServerName"), std::string("LocalServerV2"));
	loader.DoSave();

	std::make_shared<LocalTcpServer>(std::stoi(port))->Start(std::stoi(threadCount));
	//std::make_shared<TcpServerBase>(88)->Start();


	while (true)
	{

		std::this_thread::sleep_for(chrono::seconds(1));
	}
	return 0;
}