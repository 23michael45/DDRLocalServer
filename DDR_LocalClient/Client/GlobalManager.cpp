#include "GlobalManager.h"
#include "LocalClientUdpDispatcher.h"

GlobalManager::GlobalManager():m_ClientConfig("Config/Client/ClientConfig.xml")
{
}
GlobalManager::~GlobalManager()
{

}

void GlobalManager::StartUdp()
{
	while (m_spUdpClient)
	{
		std::this_thread::sleep_for(chrono::seconds(1));
	}
	m_spUdpClient = std::make_shared<UdpSocketBase>();
	m_spUdpClient->BindOnDisconnect(std::bind(&GlobalManager::OnUdpDisconnect, this, std::placeholders::_1));


	m_spUdpClient->Start();
	m_spUdpClient->GetSerializer()->BindDispatcher(std::make_shared<LocalClientUdpDispatcher>());
	m_spUdpClient->StartReceive(m_ClientConfig.GetValue<int>("UdpPort"));

}
void GlobalManager::ReleaseUdp()
{
	if (m_spUdpClient)
	{
		m_spUdpClient.reset();
	}
}

void GlobalManager::StartAudioClient(std::string ip,int port)
{

#ifdef QT_PRECOMPILED_HEADER
#else
    m_spAudioTcpClient = std::make_shared<AudioTcpClient>();
    m_spAudioTcpClient->Start(1);
    m_spAudioTcpClient->Connect(ip, std::to_string(port));
#endif
}
void GlobalManager::StopAudioClient()
{
#ifdef QT_PRECOMPILED_HEADER
#else
    if (m_spAudioTcpClient)
    {
        m_spAudioTcpClient->Stop();
    }
#endif
}


void GlobalManager::StartTcpClient()
{
	m_spTcpClient = std::make_shared<LocalTcpClient>();

}
void GlobalManager::ReleaseTcp()
{
	if (m_spTcpClient)
	{
		m_spTcpClient.reset();
	}
}
bool GlobalManager::IsUdpWorking()
{
	return m_spUdpClient != nullptr;
}
bool GlobalManager::IsTcpWorking()
{
	return m_spTcpClient != nullptr;
}
std::shared_ptr<LocalTcpClient> GlobalManager::GetTcpClient()
{
	return m_spTcpClient;
}
std::shared_ptr<UdpSocketBase> GlobalManager::GetUdpClient()
{
	return m_spUdpClient;
}
#ifdef QT_PRECOMPILED_HEADER
#else
std::shared_ptr<AudioTcpClient> GlobalManager::GetAudioTcpClient()
{

    return m_spAudioTcpClient;
}

void GlobalManager::SetServerAddr(std::string ip, std::string port)
{
	m_ServerIP = ip;
	m_ServerPort = port;
}

void GlobalManager::TryConnect(std::string ip ,std::string port)
{
	if (IsUdpWorking())
	{
		GetUdpClient()->StopReceive();
		GetUdpClient()->Stop();
		GlobalManager::Instance()->GetTcpClient()->Connect(ip, port);

	}

}
void GlobalManager::TryConnect()
{
	if (m_ServerIP.empty() || m_ServerPort.empty())
	{
		DebugLog("No Server IP Broadcast Recieved");
	}
	else
	{
		if (IsUdpWorking())
		{
			GetUdpClient()->StopReceive();
			GetUdpClient()->Stop();
			GlobalManager::Instance()->GetTcpClient()->Connect(m_ServerIP, m_ServerPort);

		}
	}
}

#endif
void GlobalManager::OnUdpDisconnect(UdpSocketBase& container)
{
	ReleaseUdp();
}
