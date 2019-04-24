#include "RemoteTcpServer.h"
#include "src/Network/TcpSocketContainer.h"
#include "src/Network/MessageSerializer.h"
#include "src/Network/BaseMessageDispatcher.h"
#include "RemoteServerDispatcher.h"
#include "RemoteServerHeadRuleRouter.h"
#include "../Managers/GlobalManager.h"
#include "../Managers/StreamProxyManager.h"

#include "src/Utility/XmlLoader.h"
#include "src/Network/TcpSocketContainer.h"
#include "../Behaviors/BaseClientBehavior.h"



RemoteServerTcpSession::RemoteServerTcpSession(asio::io_context& context) :TcpSessionBase::TcpSessionBase(context)
{
}

RemoteServerTcpSession::~RemoteServerTcpSession()
{
	if (m_SessionType== RemoteServerTcpSession::RemoteServerTcpSessionType::RST_CLIENT)
	{
		
	}
	else if (m_SessionType == RemoteServerTcpSession::RemoteServerTcpSessionType::RST_LS)
	{
		m_BindClientMap.clear();
	}
	DebugLog("RemoteServerTcpSession Destroy");
}

bool RemoteServerTcpSession::AssignRegisteLSInfo(reqRegisteLS info)
{
	m_reqRegisteLS.CopyFrom(info); 


	auto& lsmap = GlobalManager::Instance()->GetTcpServer()->GetLSMap();
	if (lsmap.find(info.udid()) != lsmap.end())
	{
		return false;
	}
	else
	{
		lsmap.insert(make_pair(info.udid(), shared_from_base()));
		m_SessionType = RemoteServerTcpSession::RemoteServerTcpSessionType::RST_LS;
		return true;
	}


}

DDRCommProto::reqRegisteLS& RemoteServerTcpSession::GetRegisteLSInfo()
{
	return m_reqRegisteLS;
}





void RemoteServerTcpSession::AssingClient(std::string username, std::shared_ptr<RemoteServerTcpSession> spClientSession)
{
	if (m_BindClientMap.find(username) == m_BindClientMap.end())
	{
		m_BindClientMap.insert(std::make_pair(username, spClientSession));
	}
}

void RemoteServerTcpSession::RemoveClient(std::string username)
{
	if (m_BindClientMap.find(username) != m_BindClientMap.end())
	{
		m_BindClientMap.erase(username);
	}
}

std::shared_ptr<RemoteServerTcpSession> RemoteServerTcpSession::GetClientSession(std::string username)
{
	if (m_BindClientMap.find(username) != m_BindClientMap.end())
	{
		return m_BindClientMap[username];
	}
	return nullptr;
}

std::map <std::string, std::shared_ptr<RemoteServerTcpSession>>& RemoteServerTcpSession::GetBindClientMap()
{
	return	m_BindClientMap;
}

void RemoteServerTcpSession::BindLS(std::string udid)
{

	auto& map = GlobalManager::Instance()->GetTcpServer()->GetLSMap();
	if (map.find(udid) != map.end())
	{
		auto spLSSession = map[udid];
		spLSSession->AssingClient(m_reqRemoteLogin.username(), shared_from_base());
		m_spBindLS = spLSSession;
	}
	else
	{

	}

}
void RemoteServerTcpSession::ReleaseLS(std::string udid)
{
	if (m_spBindLS)
	{
		if (m_spBindLS->GetRegisteLSInfo().udid() == udid)
		{

			m_spBindLS->RemoveClient(m_reqRemoteLogin.username());
			m_spBindLS.reset();


		}
	}
}

bool RemoteServerTcpSession::AssignRemoteLoginInfo(reqRemoteLogin info)
{

	auto& map = GlobalManager::Instance()->GetTcpServer()->GetClientMap();

	if (map.find(info.username()) == map.end())
	{
		auto pair = make_pair(info.username(), shared_from_base());
		map.insert(pair);

		m_SessionType = RemoteServerTcpSession::RemoteServerTcpSessionType::RST_CLIENT;

		m_reqRemoteLogin.CopyFrom(info);
		return true;
	}
	else
	{
		return false;
	}
}




std::string RemoteServerTcpSession::GetBindLSUDID()
{
	if (m_spBindLS)
	{
		return m_spBindLS->GetRegisteLSInfo().udid();
	}
	else
	{

		return "";
	}

}

DDRCommProto::reqRemoteLogin& RemoteServerTcpSession::GetRemoteLoginInfo()
{
	return m_reqRemoteLogin;
}


void RemoteServerTcpSession::OnStart()
{
	TcpSessionBase::OnStart();

	auto spClientBehavior = std::make_shared<BaseClientBehavior>();
	BindBehavior(spClientBehavior);

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
				m_ClientSessionMap.erase(username);
			}

			std::string udid = spClientSession->GetBindLSUDID();
			if (m_LSSessionMap.find(udid) != m_LSSessionMap.end())
			{
				m_LSSessionMap[udid]->RemoveClient(username);
			}

			StreamProxyManager::Instance()->ReleaseProxyDownload(username);

		}
		else if (spClientSession->GetType() == RemoteServerTcpSession::RemoteServerTcpSessionType::RST_LS)
		{
			std::string udid = spClientSession->GetRegisteLSInfo().udid();

			StreamProxyManager::Instance()->ReleaseProxyUpload(udid);

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
