#ifndef GlobalManager_h__
#define GlobalManager_h__

#include "src/Utility/XmlLoader.h"
#include "src/Network/UdpSocketBase.h"
#include "src/Network/TcpClientBase.h"
#include "src/Utility/Singleton.h"
#include "src/Utility/LocalizationLoader.h"
#include "src/Utility/GlobalManagerBase.h"
#include "VirtualServiceTcpClient.h"
#include "../Server/VirtualTcpServer.h"
using namespace DDRFramework;
class GlobalManager : public DDRFramework::CSingleton<GlobalManager>, public GlobalManagerClientBase
{
public:
	GlobalManager();
	~GlobalManager();

public:
	virtual void Init() override;
	virtual bool StartUdp() override;

	void StartTcpServer();
	void StopTcpServer();

	std::shared_ptr<VirtualTcpServer> GetTcpServer()
	{
		return m_spTcpServer;
	}


	XmlLoader GetConfig()
	{
		return m_Config;
	}
private:
	std::shared_ptr<VirtualTcpServer> m_spTcpServer;


private:


	XmlLoader m_Config;
};

#endif // GlobalManager_h__
