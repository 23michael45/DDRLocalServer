#include "GlobalManager.h"


#include "../../../Shared/src/Utility/XmlLoader.h"
#include "../../../Shared/src/Utility/CommonFunc.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>

#include "../../../Shared/proto/BaseCmd.pb.h"
#include "../../../Shared/thirdparty/cpp-sqlite3/cppsqlite3.h"
using namespace DDRCommProto;
using namespace std;

GlobalManager::GlobalManager():m_RemoteServerListConfig("Config/RemoteServer/RemoteServerConfig.xml")
{
	
}
GlobalManager::~GlobalManager()
{

}
void GlobalManager::StartTcpServer()
{
	try
	{
		int port = m_RemoteServerListConfig.GetValue<int>("TcpPort");
		std::string servername = m_RemoteServerListConfig.GetValue("ServerName");
		std::string threadCount = m_RemoteServerListConfig.GetValue("ServerThreadCount");

		//loader.SetValue(std::string("ServerName"), std::string("LocalServerV2"));
		//loader.DoSave();

		m_spTcpServer = std::make_shared<RemoteTcpServer>(port);
		m_spTcpServer->Start(std::stoi(threadCount));
	}
	catch (std::exception& e)
	{

		DebugLog("StartTcpServer Error");
	}



}
void GlobalManager::StopTcpServer()
{
	if (m_spTcpServer)
	{
		m_spTcpServer->Stop();
	}

}
