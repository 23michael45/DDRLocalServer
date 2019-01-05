#include "GlobalManager.h"
#include "LocalClientUdpDispatcher.h"
#include "LocalClientUdpProcessor.h"

GlobalManager::GlobalManager():m_Config("Config/Client/ClientConfig.xml")
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
		m_spTcpClient = std::make_shared<LocalTcpClient>();
		m_spTcpClient->Start();
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


void GlobalManager::TcpConnect()
{
	if (!m_ServerIP.empty() && !m_ServerPort.empty())
	{
		GlobalManagerClientBase::TcpConnect(m_ServerIP, m_ServerPort);
	}
}

void GlobalManager::TcpConnect(std::string ip, std::string port)
{
	GlobalManagerClientBase::TcpConnect(ip, port);
}

#endif