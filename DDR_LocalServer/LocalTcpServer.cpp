#include "LocalTcpServer.h"
#include "../../Shared/src/Network/TcpSocketContainer.h"
#include "../../Shared/src/Network/MessageSerializer.h"
#include "../../Shared/src/Network/BaseMessageDispatcher.h"
#include "LocalServerDispatcher.h"


LocalTcpServer::LocalTcpServer(int port):TcpServerBase(port)
{
}


LocalTcpServer::~LocalTcpServer()
{
}


std::shared_ptr<TcpSessionBase> LocalTcpServer::BindSerializerDispatcher()
{
	BIND_IOCONTEXT_SERIALIZER_DISPATCHER(m_IOContext, TcpSessionBase, MessageSerializer, LocalServerDispatcher)
		return spTcpSessionBase;
}
