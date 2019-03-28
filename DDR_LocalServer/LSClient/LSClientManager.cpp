#include "LSClientManager.h"


LSClientManager::LSClientManager()
{
}
LSClientManager::~LSClientManager()
{

}

void LSClientManager::Init()
{
	if (!m_spTcpClient)
	{
		m_spTcpClient = std::make_shared<LSTcpClient>();
		m_spTcpClient->Start();
	}

	if (!m_spLSBroadcastReceiveTcpClient)
	{
		m_spLSBroadcastReceiveTcpClient = std::make_shared<LSBroadcastReceiveTcpClient>();
		m_spLSBroadcastReceiveTcpClient->Start();

	}
}

//#define DebugRemoteServer
void LSClientManager::ConnectBroadcastServer()
{

#ifdef DebugRemoteServer


	LSClientManager::Instance()->TcpConnect("192.168.1.183", "8900");

#else

	std::string ip = m_GlobalConfig.GetValue("BroadcastServerIP");
	std::string port = m_GlobalConfig.GetValue("BroadcastServerPort");
	if (m_spCurrentBroadcastSession)
	{
		int count = m_spCurrentBroadcastSession.use_count();
		m_spCurrentBroadcastSession.reset();
	}

	m_spCurrentBroadcastSession = m_spLSBroadcastReceiveTcpClient->Connect(ip, port);
#endif
	
}

void LSClientManager::CloseBroadcastServer(std::vector<DDRCommProto::rspRemoteServerList_RemoteServer> servers)
{
	m_Servers = servers;
	if (m_spCurrentBroadcastSession)
	{
		m_spLSBroadcastReceiveTcpClient->Disconnect(m_spCurrentBroadcastSession);
		m_spCurrentBroadcastSession.reset();
	}
}

void LSClientManager::ConnectRemoteServer()
{
	if (m_Servers.size() > 0)
	{
		auto server = m_Servers[0];
		TcpConnect(server.ip(), std::to_string(server.port()));

	}
}

