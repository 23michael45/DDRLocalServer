#include "../Client/GlobalManager.h"
#include "../../../DDRLocalServer/DDR_LocalClient/Client/LocalClientUdpDispatcher.h"
#include "../Processors/LocalClientUdpProcessor.h"

GlobalManager::GlobalManager() :m_Config("Config/VirtualService/VirtualConfig.xml")
{

}
GlobalManager::~GlobalManager()
{
}
void GlobalManager::Init()
{
	if (!m_spUdpClient)
	{
		m_spUdpClient = std::make_shared<UdpSocketBase>();
		m_spUdpClient->BindOnDisconnect(std::bind(&GlobalManagerClientBase::OnUdpDisconnect, this, std::placeholders::_1));
		StartUdp();
	}
	if (!m_spTcpClient)
	{
		m_spTcpClient = std::make_shared<VirtualServiceTcpClient>();
		m_spTcpClient->Start(m_Config.GetValue<int>("ThreadCount"));
	}

	if (!m_spTcpServer)
	{
		StartTcpServer();
	}
}
bool GlobalManager::StartUdp()
{
	if (m_spUdpClient)
	{
		m_spUdpClient->Start();

		auto spDispatcher = std::make_shared<BaseUdpMessageDispatcher>();
		spDispatcher->AddProcessor<bcLSAddr, LocalClientUdpProcessor>();
		m_spUdpClient->GetSerializer()->BindDispatcher(spDispatcher);
		m_spUdpClient->StartReceive(m_GlobalConfig.GetValue<int>("UdpPort"));
	}
	return true;
}

void GlobalManager::StartTcpServer()
{
	try
	{
		int port = m_Config.GetValue<int>("TcpPort");
		std::string servername = m_Config.GetValue("ServerName");
		std::string threadCount = m_Config.GetValue("ServerThreadCount");



		m_spTcpServer = std::make_shared<VirtualTcpServer>(port);
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