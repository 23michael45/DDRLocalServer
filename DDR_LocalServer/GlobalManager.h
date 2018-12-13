#ifndef GlobalManager_h__
#define GlobalManager_h__

#include "../../Shared/src/Network/UdpSocketBase.h"
#include "../../Shared/src/Utility/Singleton.h"
#include "LocalTcpServer.h"

using namespace DDRFramework;
class GlobalManager : public CSingleton<GlobalManager>
{

public:
	GlobalManager();
	~GlobalManager();

	void StartTcpServer();
	void StopTcpServer();


	void StartUdpServer();
	void StopUdpServer();


	std::shared_ptr<LocalTcpServer> GetTcpServer()
	{
		return m_spTcpServer;
	}
	std::shared_ptr<UdpSocketBase> GetUdpServer()
	{
		return m_spUdpServer;
	}

private:
	std::shared_ptr<LocalTcpServer> m_spTcpServer;
	std::shared_ptr<UdpSocketBase> m_spUdpServer;
};
#endif // GlobalManager_h__
