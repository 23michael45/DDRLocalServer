#include "LocalTcpClient.h"
#include "../../Shared/src/Network/TcpSocketContainer.h"
#include "../../Shared/src/Network/MessageSerializer.h"
#include "../../Shared/src/Network/BaseMessageDispatcher.h"
#include "LocalClientDispatcher.h"


LocalTcpClient::LocalTcpClient()
{
}


LocalTcpClient::~LocalTcpClient()
{
}

std::shared_ptr<TcpClientSessionBase> LocalTcpClient::BindSerializerDispatcher()
{
	BIND_IOCONTEXT_SERIALIZER_DISPATCHER(m_IOContext, TcpClientSessionBase, MessageSerializer, LocalClientDispatcher)
		return spTcpClientSessionBase;
}
void LocalTcpClient::OnConnected(TcpSocketContainer& container)
{
	DebugLog("\nOnConnectSuccess! LocalTcpClient");
}