#include "LocalTcpServer.h"
#include "../../../Shared/src/Network/TcpSocketContainer.h"
#include "../../../Shared/src/Network/MessageSerializer.h"
#include "../../../Shared/src/Network/BaseMessageDispatcher.h"
#include "LocalServerDispatcher.h"
#include "LocalServerHeadRuleRouter.h"
#include "../Managers/GlobalManager.h"
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

	GlobalManager::Instance()->GetTcpServer()->AddSessionType(info.type(), info.username(), shared_from_base());
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
	auto spClientSession = dynamic_pointer_cast<LocalServerTcpSession>(spContainer);
	auto info = spClientSession->GetLoginInfo();
	RemoveSessionType(info.type(), info.username());
}

void LocalTcpServer::AddSessionType(eCltType type, std::string sname, std::shared_ptr<LocalServerTcpSession> sp)
{
	if (m_TypeSessionMap.find(type) == m_TypeSessionMap.end())
	{
		auto spMap = std::make_shared<std::map<std::string, std::shared_ptr<LocalServerTcpSession>>>();
		m_TypeSessionMap.insert(make_pair(type, spMap));
	}
	auto spMap = m_TypeSessionMap[type];
	if (spMap->find(sname) == spMap->end())
	{
		spMap->insert(make_pair(sname, sp));
	}
	else
	{
		DebugLog("Add LocalServerTcpSession of type:%i name:%s already Exist", type, sname.c_str());
	}
}

void LocalTcpServer::RemoveSessionType(eCltType type, std::string sname)
{
	if (m_TypeSessionMap.find(type) != m_TypeSessionMap.end())
	{
		auto spMap = m_TypeSessionMap[type];
		if (spMap->find(sname) != spMap->end())
		{
			spMap->erase(sname);
		}
		else
		{

			DebugLog("Remove LocalServerTcpSession of type:%i name:%s not Exist", type, sname.c_str());
		}
	}
	else
	{
		DebugLog("Remove LocalServerTcpSession of type:%i not Exist", type);
	}
}

std::shared_ptr<LocalServerTcpSession> LocalTcpServer::GetSessionByType(eCltType type)
{
	if (m_TypeSessionMap.find(type) != m_TypeSessionMap.end())
	{
		auto spMap = m_TypeSessionMap[type];
		if (spMap->size() > 0)
		{
			auto sp = spMap->begin()->second;
			return sp;
		}
	}
	return nullptr;
}

