#include "VirtualServiceTcpClient.h"
#include "../../../Shared/src/Network/TcpSocketContainer.h"
#include "../../../Shared/src/Network/MessageSerializer.h"
#include "../../../Shared/src/Network/BaseMessageDispatcher.h"
#include "Client/LocalClientDispatcher.h"
#include "Client/LocalClientUdpDispatcher.h"

#include "Client/HttpFileServer.h"


#include "../../../Shared/proto/BaseCmd.pb.h"
#include "../Client/GlobalManager.h"
using namespace DDRCommProto;

VirtualServiceTcpClient::VirtualServiceTcpClient()
{
}


VirtualServiceTcpClient::~VirtualServiceTcpClient()
{
	DebugLog("Destroy VirtualServiceTcpClient")
}

std::shared_ptr<TcpClientSessionBase> VirtualServiceTcpClient::BindSerializerDispatcher()
{
	BIND_IOCONTEXT_SERIALIZER_DISPATCHER(m_IOContext, TcpClientSessionBase, MessageSerializer, LocalClientDispatcher,BaseHeadRuleRouter)
		return spTcpClientSessionBase;
}
void VirtualServiceTcpClient::OnConnected(std::shared_ptr<TcpSocketContainer> spContainer)
{

	DebugLog("OnConnectSuccess! VirtualServiceTcpClient");


	StartHeartBeat();
	auto spreq = std::make_shared<reqLogin>();
	spreq->set_username(GlobalManager::Instance()->GetConfig().GetValue("ServerName"));
	spreq->set_type(eCltType::eLSMVirtual);

	Send(spreq);
	spreq.reset();

}
void VirtualServiceTcpClient::OnDisconnect(std::shared_ptr<TcpSocketContainer> spContainer)
{
	TcpClientBase::OnDisconnect(spContainer);
	//HttpFileServer::Instance()->Stop();

	GlobalManager::Instance()->StartUdp();
}
void VirtualServiceTcpClient::StartHeartBeat()
{
	/*m_HeartBeatTimerID = m_Timer.add(std::chrono::seconds(1) , [](timer_id id)
	{

	});*/

	m_HeartBeatTimerID = m_Timer.add(std::chrono::seconds(1), std::bind(&VirtualServiceTcpClient::SendHeartBeatOnce, shared_from_base(),std::placeholders::_1), std::chrono::seconds(1));
}
void VirtualServiceTcpClient::StopHeartBeat()
{
	m_Timer.remove(m_HeartBeatTimerID);
}
void VirtualServiceTcpClient::SendHeartBeatOnce(timer_id id)
{
	auto sp = std::make_shared<HeartBeat>();
	sp->set_whatever("hb");

	Send(sp);
	sp.reset();
}

void VirtualServiceTcpClient::RequestStreamInfo()
{
	auto sp = std::make_shared<reqStreamServiceInfo>();

	auto name = GlobalManager::Instance()->GetConfig().GetValue("ServerName");
	sp->set_name(name);
	
	Send(sp);
	sp.reset();
}