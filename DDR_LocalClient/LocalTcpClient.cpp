#include "LocalTcpClient.h"
#include "../../Shared/src/Network/TcpSocketContainer.h"
#include "../../Shared/src/Network/MessageSerializer.h"
#include "../../Shared/src/Network/BaseMessageDispatcher.h"
#include "LocalClientDispatcher.h"


#include "../../Shared/proto/BaseCmd.pb.h"
using namespace DDRCommProto;

LocalTcpClient::LocalTcpClient()
{
}


LocalTcpClient::~LocalTcpClient()
{
}

std::shared_ptr<TcpClientSessionBase> LocalTcpClient::BindSerializerDispatcher()
{
	BIND_IOCONTEXT_SERIALIZER_DISPATCHER(m_IOContext, TcpClientSessionBase, MessageSerializer, LocalClientDispatcher,BaseHeadRuleRouter)
		return spTcpClientSessionBase;
}
void LocalTcpClient::OnConnected(TcpSocketContainer& container)
{

	DebugLog("\nOnConnectSuccess! LocalTcpClient");
	auto spreq = std::make_shared<reqLogin>();
	spreq->set_username("LocalTcpClient_XX");

	if (m_spClient && m_spClient->IsConnected())
	{
		m_spClient->Send(spreq);
	}
	spreq.reset();

}