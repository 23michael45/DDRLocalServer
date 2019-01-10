#include "RemoteTcpServer.h"
#include "../../../Shared/src/Network/TcpSocketContainer.h"
#include "../../../Shared/src/Network/MessageSerializer.h"
#include "../../../Shared/src/Network/BaseMessageDispatcher.h"
#include "RemoteServerDispatcher.h"
#include "RemoteServerHeadRuleRouter.h"

#include "../../../Shared/src/Utility/XmlLoader.h"
#include "../../../Shared/src/Network/TcpSocketContainer.h"


RemoteServerTcpSession::RemoteServerTcpSession(asio::io_context& context) :TcpSessionBase::TcpSessionBase(context)
{
}

RemoteServerTcpSession::~RemoteServerTcpSession()
{
	DebugLog("RemoteServerTcpSession Destroy");
}

void RemoteServerTcpSession::AssignRegisteLSInfo(reqRegisteLS info)
{
	m_reqRegisteLS.CopyFrom(info); 
	m_SessionType = RemoteServerTcpSession::RemoteServerTcpSessionType::RST_LS;
}

DDRCommProto::reqRegisteLS& RemoteServerTcpSession::GetRegisteLSInfo()
{
	return m_reqRegisteLS;
}





void RemoteServerTcpSession::AssignSelectLSInfo(reqSelectLS info)
{
	m_reqSelectLS.CopyFrom(info);

}

void RemoteServerTcpSession::AssignRemoteLoginInfo(reqRemoteLogin info)
{
	m_reqRemoteLogin.CopyFrom(info);

	m_SessionType = RemoteServerTcpSession::RemoteServerTcpSessionType::RST_CLIENT;

}




DDRCommProto::reqSelectLS& RemoteServerTcpSession::GetSelectLSInfo()
{
	return m_reqSelectLS;

}

DDRCommProto::reqRemoteLogin& RemoteServerTcpSession::GetRemoteLoginInfo()
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
	BIND_IOCONTEXT_SERIALIZER_DISPATCHER(m_IOContext, RemoteServerTcpSession, MessageSerializer, RemoteServerDispatcher, RemoteServerHeadRuleRouter)
		return spRemoteServerTcpSession;
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
	auto spClientSession = dynamic_pointer_cast<RemoteServerTcpSession>(spContainer);

	if (spClientSession)
	{
		if (spClientSession->GetType() == RemoteServerTcpSession::RemoteServerTcpSessionType::RST_CLIENT)
		{
			std::string username = spClientSession->GetRemoteLoginInfo().username();
			if (m_ClientSessionMap.find(username) != m_ClientSessionMap.end())
			{
				m_LSSessionMap.erase(username);
			}
		}
		else if (spClientSession->GetType() == RemoteServerTcpSession::RemoteServerTcpSessionType::RST_LS)
		{
			std::string udid = spClientSession->GetRegisteLSInfo().udid();
			if (m_LSSessionMap.find(udid) != m_LSSessionMap.end())
			{
				m_LSSessionMap.erase(udid);
			}
		}

		

	
	}
}

std::map<std::string, std::shared_ptr<RemoteServerTcpSession>>& RemoteTcpServer::GetLSMap()
{
	return m_LSSessionMap;
}

std::map<std::string, std::shared_ptr<RemoteServerTcpSession>>& RemoteTcpServer::GetClientMap()
{
	return m_ClientSessionMap;
}

std::shared_ptr<RemoteServerTcpSession> RemoteTcpServer::GetClientSession(std::string username)
{
	if (m_ClientSessionMap.find(username) != m_ClientSessionMap.end())
	{
		return m_ClientSessionMap[username];
	}
	return nullptr;
}

std::shared_ptr<RemoteServerTcpSession> RemoteTcpServer::GetLSSession(std::string udid)
{
	if (m_LSSessionMap.find(udid) != m_LSSessionMap.end())
	{
		return m_LSSessionMap[udid];
	}
	return nullptr;
}
