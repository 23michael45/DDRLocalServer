#include "BroadcastServerTcpServer.h"
#include "../../../Shared/src/Network/TcpSocketContainer.h"
#include "../../../Shared/src/Network/MessageSerializer.h"
#include "../../../Shared/src/Network/BaseMessageDispatcher.h"
#include "BroadcastServerDispatcher.h"

#include "../../../Shared/src/Utility/XmlLoader.h"
#include "../../../Shared/src/Network/TcpSocketContainer.h"



BroadcastTcpServer::BroadcastTcpServer(int port):TcpServerBase(port)
{
}


BroadcastTcpServer::~BroadcastTcpServer()
{
}


std::shared_ptr<TcpSessionBase> BroadcastTcpServer::BindSerializerDispatcher()
{
	BIND_IOCONTEXT_SERIALIZER_DISPATCHER(m_IOContext, TcpSessionBase, MessageSerializer, BroadcastServerDispatcher, BaseHeadRuleRouter)
		return spTcpSessionBase;
}


std::shared_ptr<TcpSessionBase> BroadcastTcpServer::GetTcpSessionBySocket(tcp::socket* pSocket)
{
	if (m_SessionMap.find(pSocket) != m_SessionMap.end())
	{
		return m_SessionMap[pSocket];
	}
	return nullptr;
}

std::map<tcp::socket*, std::shared_ptr<TcpSessionBase>>& BroadcastTcpServer::GetTcpSocketContainerMap()
{
	return m_SessionMap;
}

void BroadcastTcpServer::OnSessionDisconnect(std::shared_ptr<TcpSocketContainer> spContainer)
{
	TcpServerBase::OnSessionDisconnect(spContainer);
	auto spClientSession = dynamic_pointer_cast<TcpSessionBase>(spContainer);
}
