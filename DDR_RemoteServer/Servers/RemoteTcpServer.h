#ifndef RemoteTcpServer_h__
#define RemoteTcpServer_h__


#include "../../../Shared/src/Network/TcpServerBase.h"
#include "../../../Shared/src/Utility/Singleton.h"
using namespace DDRFramework;


class RemoteServerTcpSession : public TcpSessionBase
{
	

public:
	enum RemoteServerTcpSessionType
	{
		RST_CLIENT,
		RST_LS,


	};
	RemoteServerTcpSession(asio::io_context& context);
	~RemoteServerTcpSession();

	void AssignRegisteLSInfo(reqRegisteLS info);
	reqRegisteLS& GetRegisteLSInfo();


	void AssignSelectLSInfo(reqSelectLS info);
	void AssignRemoteLoginInfo(reqRemoteLogin info);


	reqSelectLS& GetSelectLSInfo();
	reqRemoteLogin& GetRemoteLoginInfo();

	RemoteServerTcpSessionType GetType()
	{
		return m_SessionType;
	}

protected:
	reqRegisteLS m_reqRegisteLS;//LS Registe Information


	reqSelectLS m_reqSelectLS;//Client Select LS Info
	reqRemoteLogin m_reqRemoteLogin;

	RemoteServerTcpSessionType m_SessionType;
};


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


	std::map<std::string, std::shared_ptr<RemoteServerTcpSession>>& GetLSMap();
	std::map<std::string, std::shared_ptr<RemoteServerTcpSession>>& GetClientMap();

	std::shared_ptr<RemoteServerTcpSession> GetClientSession(std::string username);
	std::shared_ptr<RemoteServerTcpSession> GetLSSession(std::string udid);
protected:

	std::map<std::string, std::shared_ptr<RemoteServerTcpSession>> m_LSSessionMap;
	std::map<std::string, std::shared_ptr<RemoteServerTcpSession>> m_ClientSessionMap;

};


#endif // RemoteTcpServer_h__
