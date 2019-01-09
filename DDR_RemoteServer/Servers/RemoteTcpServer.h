#ifndef RemoteTcpServer_h__
#define RemoteTcpServer_h__


#include "../../../Shared/src/Network/TcpServerBase.h"
#include "../../../Shared/src/Utility/Singleton.h"
using namespace DDRFramework;




class RemoteTcpServer : public TcpServerBase
{
public:
	RemoteTcpServer(int port);
	~RemoteTcpServer();


	virtual std::shared_ptr<TcpSessionBase> BindSerializerDispatcher();

	std::shared_ptr<TcpSessionBase> GetTcpSessionBySocket(tcp::socket* pSocket);
	std::map<tcp::socket*,std::shared_ptr<TcpSessionBase>>& GetTcpSocketContainerMap();

	virtual void OnSessionDisconnect(std::shared_ptr<TcpSocketContainer> spContainer) override;


	auto shared_from_base() {
		return std::static_pointer_cast<RemoteTcpServer>(shared_from_this());
	}
};


#endif // RemoteTcpServer_h__
