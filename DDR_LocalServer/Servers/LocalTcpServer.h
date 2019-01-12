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

	auto shared_from_base() {
		return std::static_pointer_cast<LocalServerTcpSession>(shared_from_this());
	}
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

	void AddSessionType(eCltType type, std::string sname, std::shared_ptr<LocalServerTcpSession> sp);
	void RemoveSessionType(eCltType type, std::string sname);
	std::shared_ptr<LocalServerTcpSession> GetSessionByType(eCltType type);


	auto shared_from_base() {
		return std::static_pointer_cast<LocalTcpServer>(shared_from_this());
	}

protected:
	std::map<eCltType, std::shared_ptr<std::map<std::string, std::shared_ptr<LocalServerTcpSession>>>> m_TypeSessionMap;
};


#endif // LocalTcpServer_h__