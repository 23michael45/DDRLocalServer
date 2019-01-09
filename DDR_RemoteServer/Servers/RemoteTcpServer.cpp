#include "RemoteTcpServer.h"
#include "../../../Shared/src/Network/TcpSocketContainer.h"
#include "../../../Shared/src/Network/MessageSerializer.h"
#include "../../../Shared/src/Network/BaseMessageDispatcher.h"
#include "RemoteServerDispatcher.h"

#include "../../../Shared/src/Utility/XmlLoader.h"
#include "../../../Shared/src/Network/TcpSocketContainer.h"



RemoteTcpServer::RemoteTcpServer(int port):TcpServerBase(port)
{
}


RemoteTcpServer::~RemoteTcpServer()
{
}


std::shared_ptr<TcpSessionBase> RemoteTcpServer::BindSerializerDispatcher()
{
	BIND_IOCONTEXT_SERIALIZER_DISPATCHER(m_IOContext, TcpSessionBase, MessageSerializer, RemoteServerDispatcher, BaseHeadRuleRouter)
		return spTcpSessionBase;
}


std::shared_ptr<TcpSessionBase> RemoteTcpServer::GetTcpSessionBySocket(tcp::socket* pSocket)
{
	if (m_SessionMap.find(pSocket) != m_SessionMap.end())
	{
		return m_SessionMap[pSocket];
	}
	return nullptr;
}

std::map<tcp::socket*, std::shared_ptr<TcpSessionBase>>& RemoteTcpServer::GetTcpSocketContainerMap()
{
	return m_SessionMap;
}

void RemoteTcpServer::OnSessionDisconnect(std::shared_ptr<TcpSocketContainer> spContainer)
{
	TcpServerBase::OnSessionDisconnect(spContainer);
	auto spClientSession = dynamic_pointer_cast<TcpSessionBase>(spContainer);
}
