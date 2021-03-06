#include "GlobalManager.h"


#include "src/Utility/XmlLoader.h"
#include "src/Utility/CommonFunc.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>

#include "proto/BaseCmd.pb.h"
#include "thirdparty/cpp-sqlite3/cppsqlite3.h"
using namespace DDRCommProto;
using namespace std;

GlobalManager::GlobalManager():m_BroadcastServerConfig("Config/BroadcastServer/BroadcastServerConfig.xml"), m_ServerListConfig("Config/BroadcastServer/ServerListConfig.xml")
{
	int count = m_ServerListConfig.GetElementCount();
	
	for (int i = 0 ; i<count;i++)
	{
		DDRCommProto::rspRemoteServerList::RemoteServer server;
		server.set_name(m_ServerListConfig.GetValue(i, "Name"));
		server.set_ip(m_ServerListConfig.GetValue(i, "IP"));
		server.set_port(m_ServerListConfig.GetValue<int>(i, "Port"));
		m_ServerList.push_back(server);
	}
	
}
GlobalManager::~GlobalManager()
{

}
void GlobalManager::StartTcpServer()
{
	try
	{
		int port = m_BroadcastServerConfig.GetValue<int>("TcpPort");
		std::string servername = m_BroadcastServerConfig.GetValue("ServerName");
		std::string threadCount = m_BroadcastServerConfig.GetValue("ServerThreadCount");

		//loader.SetValue(std::string("ServerName"), std::string("LocalServerV2"));
		//loader.DoSave();

		m_spTcpServer = std::make_shared<BroadcastTcpServer>(port);
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
