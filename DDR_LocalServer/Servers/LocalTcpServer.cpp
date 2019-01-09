#include "LocalTcpServer.h"
#include "../../../Shared/src/Network/TcpSocketContainer.h"
#include "../../../Shared/src/Network/MessageSerializer.h"
#include "../../../Shared/src/Network/BaseMessageDispatcher.h"
#include "LocalServerDispatcher.h"
#include "LocalServerHeadRuleRouter.h"

#include "../../../Shared/src/Utility/XmlLoader.h"


LocalServerTcpSession::LocalServerTcpSession(asio::io_context& context) :TcpSessionBase::TcpSessionBase(context)
{
}

LocalServerTcpSession::~LocalServerTcpSession()
{
	DebugLog("LocalServerTcpSession Destroy");
}

void LocalServerTcpSession::AssignLoginInfo(reqLogin info)
{
	m_reqLoginInfo.CopyFrom(info);
}

DDRCommProto::reqLogin& LocalServerTcpSession::GetLoginInfo()
{
	return m_reqLoginInfo;
}





LocalTcpServer::LocalTcpServer(int port):TcpServerBase(port)
{
}


LocalTcpServer::~LocalTcpServer()
{
}


std::shared_ptr<TcpSessionBase> LocalTcpServer::BindSerializerDispatcher()
{
	BIND_IOCONTEXT_SERIALIZER_DISPATCHER(m_IOContext, LocalServerTcpSession, MessageSerializer, LocalServerDispatcher, LocalServerHeadRuleRouter)
		return spLocalServerTcpSession;
}


std::shared_ptr<TcpSessionBase> LocalTcpServer::GetTcpSessionBySocket(tcp::socket* pSocket)
{
	if (m_SessionMap.find(pSocket) != m_SessionMap.end())
	{
		return m_SessionMap[pSocket];
	}
	return nullptr;
}

std::map<tcp::socket*, std::shared_ptr<TcpSessionBase>>& LocalTcpServer::GetTcpSocketContainerMap()
{
	return m_SessionMap;
}

void LocalTcpServer::OnSessionDisconnect(std::shared_ptr<TcpSocketContainer> spContainer)
{
	TcpServerBase::OnSessionDisconnect(spContainer);
	auto spClientSession = dynamic_pointer_cast<TcpSessionBase>(spContainer);
}


