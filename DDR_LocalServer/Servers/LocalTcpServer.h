#ifndef LocalTcpServer_h__
#define LocalTcpServer_h__

#include "../../../Shared/src/Network/TcpServerBase.h"
#include "../../../Shared/src/Utility/Singleton.h"
using namespace DDRFramework;


class LocalServerTcpSession : public TcpSessionBase
{
public:
	LocalServerTcpSession(asio::io_context& context);
	~LocalServerTcpSession();


	void AssignLoginInfo(reqLogin info);
	reqLogin& GetLoginInfo();
protected:
	reqLogin m_reqLoginInfo;//Login Information
};

class LocalTcpServer : public TcpServerBase
{
public:
	LocalTcpServer(int port);
	~LocalTcpServer();


	virtual std::shared_ptr<TcpSessionBase> BindSerializerDispatcher();

	std::shared_ptr<TcpSessionBase> GetTcpSessionBySocket(tcp::socket* pSocket);
	std::map<tcp::socket*,std::shared_ptr<TcpSessionBase>>& GetTcpSocketContainerMap();

	virtual void OnSessionDisconnect(std::shared_ptr<TcpSocketContainer> spContainer) override;


	auto shared_from_base() {
		return std::static_pointer_cast<LocalTcpServer>(shared_from_this());
	}
};


#endif // LocalTcpServer_h__