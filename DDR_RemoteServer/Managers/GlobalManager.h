#ifndef GlobalManager_h__
#define GlobalManager_h__

#include "../../../Shared/src/Network/UdpSocketBase.h"
#include "../../../Shared/src/Utility/Singleton.h"
#include "../../../Shared/src/Utility/XmlLoader.h"
#include "../../../Shared/src/Utility/LocalizationLoader.h"
#include "../../../Shared/src/Utility/GlobalManagerBase.h"
#include "../../../Shared/thirdparty/cpp-sqlite3/cppsqlite3.h"
#include "../Servers/RemoteTcpServer.h"

#include "../../../Shared/proto/BaseCmd.pb.h"

using namespace DDRFramework;


class DBManager;
class GlobalManager : public CSingleton<GlobalManager>,public GlobalManagerBase
{

public:
	GlobalManager();
	~GlobalManager();

	void StartTcpServer();
	void StopTcpServer();



	std::shared_ptr<RemoteTcpServer> GetTcpServer()
	{
		return m_spTcpServer;
	}

	XmlLoader& GetRemoteServerConfig()
	{
		return m_RemoteServerListConfig;
	}
	std::vector<DDRCommProto::rspRemoteServerList::RemoteServer>& GetServerVec()
	{
		return m_ServerList;
	}

private:
	std::shared_ptr<RemoteTcpServer> m_spTcpServer;

	XmlLoader m_RemoteServerListConfig;

	std::vector<DDRCommProto::rspRemoteServerList::RemoteServer> m_ServerList;
};


class DBManager : public CSingleton<DBManager>
{
public:
	DBManager();
	~DBManager();

	bool Open(std::string& filename);
	bool Remove(std::string& filename);

	bool VerifyUser(std::string username, std::string pwd);

	std::vector<std::string> GetUserRobotList(std::string username);

private:
	bool CreateUserTable();
	bool CodeSample();

	std::string m_FileName;
	CppSQLite3DB m_DB;
};


#endif // GlobalManager_h__
