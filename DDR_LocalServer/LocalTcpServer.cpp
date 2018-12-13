#include "LocalTcpServer.h"
#include "../../Shared/src/Network/TcpSocketContainer.h"
#include "../../Shared/src/Network/MessageSerializer.h"
#include "../../Shared/src/Network/BaseMessageDispatcher.h"
#include "LocalServerDispatcher.h"
#include "LocalServerHeadRuleRouter.h"

#include "../../Shared/src/Utility/XmlLoader.h"



LocalTcpServer::LocalTcpServer(int port):TcpServerBase(port)
{
}


LocalTcpServer::~LocalTcpServer()
{
}


std::shared_ptr<TcpSessionBase> LocalTcpServer::BindSerializerDispatcher()
{
	BIND_IOCONTEXT_SERIALIZER_DISPATCHER(m_IOContext, TcpSessionBase, MessageSerializer, LocalServerDispatcher, LocalServerHeadRuleRouter)
		return spTcpSessionBase;
}


std::shared_ptr<TcpSessionBase> LocalTcpServer::GetTcpSessionByIP(std::string ip)
{
	if (m_SessionMap.find(ip) != m_SessionMap.end())
	{
		return m_SessionMap[ip];
	}
	return nullptr;
}

std::map<std::string, std::shared_ptr<TcpSessionBase>>& LocalTcpServer::GetTcpSocketContainerMap()
{
	return m_SessionMap;
}
