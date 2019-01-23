#ifndef BroadcastServerTcpServer_h__
#define BroadcastServerTcpServer_h__



#include "../../../Shared/src/Network/TcpServerBase.h"
#include "../../../Shared/src/Utility/Singleton.h"
using namespace DDRFramework;




class BroadcastTcpServer : public TcpServerBase
{
public:
	BroadcastTcpServer(int port);
	~BroadcastTcpServer();


	virtual std::shared_ptr<TcpSessionBase> BindSerializerDispatcher();

	std::shared_ptr<TcpSessionBase> GetTcpSessionBySocket(tcp::socket* pSocket);
	std::map<tcp::socket*,std::shared_ptr<TcpSessionBase>>& GetTcpSocketContainerMap();

	virtual void OnSessionDisconnect(std::shared_ptr<TcpSocketContainer> spContainer) override;

	SHARED_FROM_BASE(BroadcastTcpServer)

};

#endif // BroadcastServerTcpServer_h__