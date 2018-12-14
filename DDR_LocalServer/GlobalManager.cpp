#include "GlobalManager.h"


#include "../../Shared/src/Utility/XmlLoader.h"
#include "LocalServerUdpDispatcher.h"
#include <thread>
#include <chrono>

#include "../../Shared/proto/BaseCmd.pb.h"
using namespace DDRCommProto;


GlobalManager::GlobalManager()
{

}
GlobalManager::~GlobalManager()
{

}
void GlobalManager::StartTcpServer()
{

	XmlLoader loader("Config/LocalServerConfig.xml");
	std::string port = loader.GetValue("TcpPort");
	std::string servername = loader.GetValue("ServerName");
	std::string threadCount = loader.GetValue("ServerThreadCount");

	//loader.SetValue(std::string("ServerName"), std::string("LocalServerV2"));
	//loader.DoSave();

	m_spTcpServer = std::make_shared<LocalTcpServer>(std::stoi(port));
	m_spTcpServer->Start(std::stoi(threadCount));



}
void GlobalManager::StopTcpServer()
{
	if (m_spTcpServer)
	{
		m_spTcpServer->Stop();
	}

}

void GlobalManager::StartUdpServer()
{
		auto bc = std::make_shared<bcLSAddr>();
		bc->set_name("ServerNameRobotX");


		XmlLoader loader("Config/LocalServerConfig.xml");
		std::string port = loader.GetValue("UdpPort");
		bc->set_port(std::stoi(port));

		asio::io_service io_service;
		tcp::resolver resolver(io_service);
		tcp::resolver::query query(asio::ip::host_name(), "");
		tcp::resolver::iterator iter = resolver.resolve(query);
		tcp::resolver::iterator end; // End marker.


		while (iter != end)
		{
			tcp::endpoint ep = *iter++;
			bc->add_ips(ep.address().to_string());
		}

		m_spUdpServer = std::make_shared<UdpSocketBase>();

		m_spUdpServer->Start();
		m_spUdpServer->GetSerializer()->BindDispatcher(std::make_shared<LocalServerUdpDispatcher>());

		m_spUdpServer->StartBroadcast(28888, bc, 2000);
		//spUdp->StartReceive(28888);


		//spUdp->StopReceive();
		
}
void GlobalManager::StopUdpServer()
{
	if (m_spUdpServer)
	{
		m_spUdpServer->StopBroadcast();
		m_spUdpServer->Stop();
		m_spUdpServer.reset();

	}
}
