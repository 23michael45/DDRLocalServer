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
