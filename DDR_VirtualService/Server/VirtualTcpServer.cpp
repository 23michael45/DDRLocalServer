#include "VirtualTcpServer.h"
#include "src/Network/TcpSocketContainer.h"
#include "src/Network/MessageSerializer.h"
#include "src/Network/BaseMessageDispatcher.h"
#include "VirtualServerDispatcher.h"
#include "src/Utility/XmlLoader.h"


VirtualServerTcpSession::VirtualServerTcpSession(asio::io_context& context) :TcpSessionBase::TcpSessionBase(context)
{
}

VirtualServerTcpSession::~VirtualServerTcpSession()
{
	DebugLog("VirtualServerTcpSession Destroy");
}




VirtualTcpServer::VirtualTcpServer(int port):TcpServerBase(port)
{
}


VirtualTcpServer::~VirtualTcpServer()
{
}


std::shared_ptr<TcpSessionBase> VirtualTcpServer::BindSerializerDispatcher()
{
	BIND_IOCONTEXT_SERIALIZER_DISPATCHER(m_IOContext, VirtualServerTcpSession, MessageSerializer, VirtualServerDispatcher, BaseHeadRuleRouter)
		return spVirtualServerTcpSession;
}


std::shared_ptr<TcpSessionBase> VirtualTcpServer::GetTcpSessionBySocket(tcp::socket* pSocket)
{
	if (m_SessionMap.find(pSocket) != m_SessionMap.end())
	{
		return m_SessionMap[pSocket];
	}
	return nullptr;
}

std::map<tcp::socket*, std::shared_ptr<TcpSessionBase>>& VirtualTcpServer::GetTcpSocketContainerMap()
{
	return m_SessionMap;
}

void VirtualTcpServer::OnSessionDisconnect(std::shared_ptr<TcpSocketContainer> spContainer)
{
	TcpServerBase::OnSessionDisconnect(spContainer);
	auto spClientSession = dynamic_pointer_cast<VirtualServerTcpSession>(spContainer);
	
	

}


