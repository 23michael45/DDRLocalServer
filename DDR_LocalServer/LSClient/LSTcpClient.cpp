#include "LSTcpClient.h"
#include "../../../Shared/src/Network/TcpSocketContainer.h"
#include "../../../Shared/src/Network/MessageSerializer.h"
#include "../../../Shared/src/Network/BaseMessageDispatcher.h"
#include "LSClientDispatcher.h"


#include "../../../Shared/proto/BaseCmd.pb.h"
#include "../Managers/GlobalManager.h"
using namespace DDRCommProto;

LSClient::LSClient()
{
}


LSClient::~LSClient()
{
	DebugLog("LocalTcpClient Destroy")
}

std::shared_ptr<TcpClientSessionBase> LSClient::BindSerializerDispatcher()
{
	BIND_IOCONTEXT_SERIALIZER_DISPATCHER(m_IOContext, TcpClientSessionBase, MessageSerializer, LSClientDispatcher,BaseHeadRuleRouter)
		return spTcpClientSessionBase;
}
void LSClient::OnConnected(std::shared_ptr<TcpSocketContainer> spContainer)
{

	DebugLog("OnConnectSuccess! LocalTcpClient");
	auto spreq = std::make_shared<reqLogin>();
	spreq->set_username("LocalTcpClient_XX");
	spreq->set_type(eLocalServer);
	spreq->set_username("admin");
	spreq->set_userpwd("admin");

	if (IsConnected())
	{
		Send(spreq);
	}
	spreq.reset();


}
void LSClient::OnDisconnect(std::shared_ptr<TcpSocketContainer> spContainer)
{
	TcpClientBase::OnDisconnect(spContainer);
}

void LSClient::StartHeartBeat()
{
	/*m_HeartBeatTimerID = m_Timer.add(std::chrono::seconds(1) , [](timer_id id)
	{

	});*/

	m_HeartBeatTimerID = m_Timer.add(std::chrono::seconds(1), std::bind(&LSClient::SendHeartBeatOnce, shared_from_base(),std::placeholders::_1), std::chrono::seconds(1));


}
void LSClient::StopHeartBeat()
{
	m_Timer.remove(m_HeartBeatTimerID);
}
void LSClient::SendHeartBeatOnce(timer_id id)
{
	HeartBeat hb;
	auto sphb = std::make_shared<HeartBeat>();
	sphb->set_whatever("hb");

	if (IsConnected())
	{
		Send(sphb);
	}
	sphb.reset();
}