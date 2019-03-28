#include "LSTcpClient.h"
#include "../../../Shared/src/Network/TcpSocketContainer.h"
#include "../../../Shared/src/Network/MessageSerializer.h"
#include "../../../Shared/src/Network/BaseMessageDispatcher.h"
#include "LSClientDispatcher.h"
#include "LSRemoteClientHeadRuleRouter.h"

#include "../../../Shared/proto/BaseCmd.pb.h"
#include "../Managers/GlobalManager.h"
#include "LSClientManager.h"
using namespace DDRCommProto;

LSTcpClient::LSTcpClient()
{
}


LSTcpClient::~LSTcpClient()
{
	DebugLog("LSTcpClient Destroy")
}

std::shared_ptr<TcpClientSessionBase> LSTcpClient::BindSerializerDispatcher()
{
	BIND_IOCONTEXT_SERIALIZER_DISPATCHER(m_IOContext, TcpClientSessionBase, MessageSerializer, LSClientDispatcher, LSRemoteClientHeadRuleRouter)
		return spTcpClientSessionBase;
}
void LSTcpClient::OnConnected(std::shared_ptr<TcpSocketContainer> spContainer)
{
	StartHeartBeat();
	DebugLog("OnConnectSuccess! LSTcpClient");
	RegisteToRemote();

}
void LSTcpClient::OnDisconnect(std::shared_ptr<TcpSocketContainer> spContainer)
{
	TcpClientBase::OnDisconnect(spContainer);
	LSClientManager::Instance()->ConnectBroadcastServer();

}

void LSTcpClient::OnConnectTimeout(std::shared_ptr<TcpSocketContainer> spContainer)
{
	TcpClientBase::OnDisconnect(spContainer);
	LSClientManager::Instance()->ConnectBroadcastServer();

}


void LSTcpClient::StartHeartBeat()
{
	/*m_HeartBeatTimerID = m_Timer.add(std::chrono::seconds(1) , [](timer_id id)
	{

	});*/

	m_HeartBeatTimerID = m_Timer.add(std::chrono::seconds(1), std::bind(&LSTcpClient::SendHeartBeatOnce, shared_from_base(),std::placeholders::_1), std::chrono::seconds(1));


}
void LSTcpClient::StopHeartBeat()
{
	m_Timer.remove(m_HeartBeatTimerID);
}

void LSTcpClient::RegisteToRemote()
{
	std::string cpuhash = getCpuHash();

	auto spreq = std::make_shared<reqRegisteLS>();
	spreq->set_udid(cpuhash);
	spreq->set_name(GlobalManager::Instance()->GetGlobalConfig().GetValue("RobotID"));

	Send(spreq);
}

void LSTcpClient::SendHeartBeatOnce(timer_id id)
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






LSBroadcastReceiveTcpClient::LSBroadcastReceiveTcpClient()
{
}


LSBroadcastReceiveTcpClient::~LSBroadcastReceiveTcpClient()
{
	DebugLog("LSBroadcastReceiveTcpClient Destroy")
}

std::shared_ptr<TcpClientSessionBase> LSBroadcastReceiveTcpClient::BindSerializerDispatcher()
{
	BIND_IOCONTEXT_SERIALIZER_DISPATCHER(m_IOContext, TcpClientSessionBase, MessageSerializer, LSBroadcastReceiveClientDispatcher, BaseHeadRuleRouter)
		return spTcpClientSessionBase;
}
void LSBroadcastReceiveTcpClient::OnConnected(std::shared_ptr<TcpSocketContainer> spContainer)
{

	DebugLog("OnConnectSuccess! LSBroadcastReceiveTcpClient");
	auto spreq = std::make_shared<reqRemoteServerList>();
	spreq->set_fromip(spContainer->GetSocket().remote_endpoint().address().to_string());
	if (IsConnected())
	{
		Send(spreq);
	}
	spreq.reset();


}
void LSBroadcastReceiveTcpClient::OnDisconnect(std::shared_ptr<TcpSocketContainer> spContainer)
{
	TcpClientBase::OnDisconnect(spContainer);
}
void LSBroadcastReceiveTcpClient::OnConnectTimeout(std::shared_ptr<TcpSocketContainer> spContainer)
{
	TcpClientBase::OnDisconnect(spContainer);
	LSClientManager::Instance()->ConnectBroadcastServer();

}

void LSBroadcastReceiveTcpClient::OnConnectFailed(std::shared_ptr<TcpSocketContainer> spContainer)
{
	std::this_thread::sleep_for(std::chrono::seconds(3));
	LSClientManager::Instance()->ConnectBroadcastServer();

}