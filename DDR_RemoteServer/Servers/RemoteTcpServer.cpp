#include "RemoteTcpServer.h"
#include "../../../Shared/src/Network/TcpSocketContainer.h"
#include "../../../Shared/src/Network/MessageSerializer.h"
#include "../../../Shared/src/Network/BaseMessageDispatcher.h"
#include "RemoteServerDispatcher.h"

#include "../../../Shared/src/Utility/XmlLoader.h"
#include "../../../Shared/src/Network/TcpSocketContainer.h"


RemoteServerTcpSession::RemoteServerTcpSession(asio::io_context& context) :TcpSessionBase::TcpSessionBase(context)
{
}

RemoteServerTcpSession::~RemoteServerTcpSession()
{
	DebugLog("LocalServerTcpSession Destroy");
}

RemoteServerTcpLSSession::RemoteServerTcpLSSession(asio::io_context& context) :RemoteServerTcpSession::RemoteServerTcpSession(context)
{
}

RemoteServerTcpLSSession::~RemoteServerTcpLSSession()
{
	DebugLog("RemoteServerTcpLSSession Destroy");
}
void RemoteServerTcpLSSession::AssignRegisteLSInfo(reqRegisteLS info)
{
	m_reqRegisteLS.CopyFrom(info);
}

DDRCommProto::reqRegisteLS& RemoteServerTcpLSSession::GetRegisteLSInfo()
{
	return m_reqRegisteLS;
}


RemoteServerTcpClientSession::RemoteServerTcpClientSession(asio::io_context& context) :RemoteServerTcpSession::RemoteServerTcpSession(context)
{
}

RemoteServerTcpClientSession::~RemoteServerTcpClientSession()
{
	DebugLog("RemoteServerTcpClientSession Destroy");
}






void RemoteServerTcpClientSession::AssignSelectLSInfo(reqSelectLS info)
{
	m_reqSelectLS.CopyFrom(info);

}

void RemoteServerTcpClientSession::AssignRemoteLoginInfo(reqRemoteLogin info)
{
	m_reqRemoteLogin.CopyFrom(info);

}




DDRCommProto::reqSelectLS& RemoteServerTcpClientSession::GetSelectLSInfo()
{
	return m_reqSelectLS;

}

DDRCommProto::reqRemoteLogin& RemoteServerTcpClientSession::GetRemoteLoginInfo()
{
	return m_reqRemoteLogin;
}

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

std::map<std::string, std::shared_ptr<RemoteServerTcpLSSession>>& RemoteTcpServer::GetLSMap()
{
	return m_LSSessionMap;
}

std::map<std::string, std::shared_ptr<RemoteServerTcpClientSession>>& RemoteTcpServer::GetClientMap()
{
	return m_ClientSessionMap;
}
