#ifndef GlobalManager_h__
#define GlobalManager_h__

#include "src/Network/UdpSocketBase.h"
#include "src/Utility/Singleton.h"
#include "src/Utility/XmlLoader.h"
#include "src/Utility/LocalizationLoader.h"
#include "src/Utility/GlobalManagerBase.h"
#include "thirdparty/cpp-sqlite3/cppsqlite3.h"
#include "../Servers/BroadcastServerTcpServer.h"

#include "../../proto/RemoteCmd.pb.h"
#include "proto/BaseCmd.pb.h"

using namespace DDRFramework;


class DBManager;
class GlobalManager : public CSingleton<GlobalManager>,public GlobalManagerBase
{

public:
	GlobalManager();
	~GlobalManager();

	void StartTcpServer();
	void StopTcpServer();



	std::shared_ptr<BroadcastTcpServer> GetTcpServer()
	{
		return m_spTcpServer;
	}

	XmlLoader& GetBroadcastServerConfig()
	{
		return m_BroadcastServerConfig;
	}
	std::vector<DDRCommProto::rspRemoteServerList::RemoteServer>& GetServerVec()
	{
		return m_ServerList;
	}

private:
	std::shared_ptr<BroadcastTcpServer> m_spTcpServer;

	XmlLoader m_BroadcastServerConfig;
	XmlLoader m_ServerListConfig;

	std::vector<DDRCommProto::rspRemoteServerList::RemoteServer> m_ServerList;
};



#endif // GlobalManager_h__
