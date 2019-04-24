#ifndef LocalTcpServer_h__
#define LocalTcpServer_h__

#include "src/Network/TcpServerBase.h"
#include "src/Utility/Singleton.h"
using namespace DDRFramework;


class VirtualServerTcpSession : public TcpSessionBase
{
public:
	VirtualServerTcpSession(asio::io_context& context);
	~VirtualServerTcpSession();




	SHARED_FROM_BASE(VirtualServerTcpSession)



protected:


};

class VirtualTcpServer : public TcpServerBase
{
public:
	VirtualTcpServer(int port);
	~VirtualTcpServer();


	virtual std::shared_ptr<TcpSessionBase> BindSerializerDispatcher();

	std::shared_ptr<TcpSessionBase> GetTcpSessionBySocket(tcp::socket* pSocket);
	std::map<tcp::socket*,std::shared_ptr<TcpSessionBase>>& GetTcpSocketContainerMap();

	virtual void OnSessionDisconnect(std::shared_ptr<TcpSocketContainer> spContainer) override;



	SHARED_FROM_BASE(VirtualTcpServer)

protected:

};


#endif // LocalTcpServer_h__