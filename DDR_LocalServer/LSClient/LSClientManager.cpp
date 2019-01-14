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
}

#define DebugRemoteServer
void LSClientManager::ConnectBroadcastServer()
{

#ifdef DebugRemoteServer


	LSClientManager::Instance()->TcpConnect("192.168.1.183", "8900");

#else
	m_spLSBroadcastReceiveTcpClient = std::make_shared<LSBroadcastReceiveTcpClient>();
	m_spLSBroadcastReceiveTcpClient->Start();

	std::string ip = m_GlobalConfig.GetValue("BroadcastServerIP");
	std::string port = m_GlobalConfig.GetValue("BroadcastServerPort");
	m_spLSBroadcastReceiveTcpClient->Connect(ip, port);
#endif
	
}

void LSClientManager::CloseBroadcastServer()
{
	if (m_spLSBroadcastReceiveTcpClient)
	{
		m_spLSBroadcastReceiveTcpClient->Stop();
	}
}
