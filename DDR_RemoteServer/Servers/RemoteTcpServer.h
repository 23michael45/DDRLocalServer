#ifndef RemoteTcpServer_h__
#define RemoteTcpServer_h__


#include "../../../Shared/src/Network/TcpServerBase.h"
#include "../../../Shared/src/Utility/Singleton.h"
using namespace DDRFramework;


class RemoteServerTcpSession : public TcpSessionBase
{
public:
	RemoteServerTcpSession(asio::io_context& context);
	~RemoteServerTcpSession();
};


class RemoteServerTcpLSSession : public RemoteServerTcpSession
{
public:
	RemoteServerTcpLSSession(asio::io_context& context);
	~RemoteServerTcpLSSession();


	void AssignRegisteLSInfo(reqRegisteLS info);
	reqRegisteLS& GetRegisteLSInfo();


protected:
	reqRegisteLS m_reqRegisteLS;//LS Registe Information

};

class RemoteServerTcpClientSession : public RemoteServerTcpSession
{
public:
	RemoteServerTcpClientSession(asio::io_context& context);
	~RemoteServerTcpClientSession();

	void AssignSelectLSInfo(reqSelectLS info);
	void AssignRemoteLoginInfo(reqRemoteLogin info);


	reqSelectLS& GetSelectLSInfo();
	reqRemoteLogin& GetRemoteLoginInfo();

protected:
	reqSelectLS m_reqSelectLS;//Client Select LS Info
	reqRemoteLogin m_reqRemoteLogin;
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


	std::map<std::string, std::shared_ptr<RemoteServerTcpLSSession>>& GetLSMap();
	std::map<std::string, std::shared_ptr<RemoteServerTcpClientSession>>& GetClientMap();


protected:

	std::map<std::string, std::shared_ptr<RemoteServerTcpLSSession>> m_LSSessionMap;
	std::map<std::string, std::shared_ptr<RemoteServerTcpClientSession>> m_ClientSessionMap;

};


#endif // RemoteTcpServer_h__
