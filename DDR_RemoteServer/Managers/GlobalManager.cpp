#include "GlobalManager.h"


#include "src/Utility/XmlLoader.h"
#include "src/Utility/CommonFunc.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>

#include "proto/BaseCmd.pb.h"
#include "thirdparty/cpp-sqlite3/cppsqlite3.h"
using namespace DDRCommProto;
using namespace std;

GlobalManager::GlobalManager():m_RemoteServerListConfig("Config/RemoteServer/RemoteServerConfig.xml")
{
	std::string dbfile = m_RemoteServerListConfig.GetValue("DatabaseFile");

	if (!dbfile.empty())
	{

		DBManager::Instance()->Open(dbfile);
	}
	else
	{
		DebugLog("DBFile Load Error");
	}
	
	std::string HttpFileServerBaseDir = m_RemoteServerListConfig.GetValue("HttpFileServerBaseDir");

}
GlobalManager::~GlobalManager()
{

}
void GlobalManager::StartTcpServer()
{
	try
	{
		int port = m_RemoteServerListConfig.GetValue<int>("TcpPort");
		std::string servername = m_RemoteServerListConfig.GetValue("ServerName");
		std::string threadCount = m_RemoteServerListConfig.GetValue("ServerThreadCount");

		//loader.SetValue(std::string("ServerName"), std::string("LocalServerV2"));
		//loader.DoSave();

		m_spTcpServer = std::make_shared<RemoteTcpServer>(port);
		m_spTcpServer->Start(std::stoi(threadCount));
	}
	catch (std::exception& e)
	{

		DebugLog("StartTcpServer Error");
	}



}
void GlobalManager::StopTcpServer()
{
	if (m_spTcpServer)
	{
		m_spTcpServer->Stop();
	}

}




DBManager::DBManager()
{

}
DBManager::~DBManager()
{

}

bool DBManager::Open(std::string& filename)
{
	m_FileName = filename;
	std::ifstream ifs(filename.c_str());
	if (!ifs.good())
	{
		std::ofstream outfile(filename);
		outfile.close();

		m_DB.open(m_FileName.c_str());
		CreateUserTable();
		m_DB.close();
	}

	try
	{
		int i, fld;
		time_t tmStart, tmEnd;
		DebugLog("SQLite Version: %s", m_DB.SQLiteVersion());
		m_DB.open(m_FileName.c_str());

	}
	catch (CppSQLite3Exception& e)
	{
		cerr << e.errorCode() << ":" << e.errorMessage() << endl;
		return false;
	}
	return true;
}
bool DBManager::Remove(std::string& filename)
{
	remove(filename.c_str());
	return true;
}


bool DBManager::CreateUserTable()
{
	try
	{
		int nRows = 0;

		m_DB.execDML("create table user(username string, userpwd string);");
		
		nRows = m_DB.execDML("insert into user values ('admin', 'admin',0);");
		nRows = m_DB.execDML("insert into user values ('admin_pc', 'admin_pc',0);");
		nRows = m_DB.execDML("insert into user values ('admin_android', 'admin_android',0);");


		m_DB.execDML("create table user2robot(username string, udid string,priority int);");
		
		nRows = m_DB.execDML("insert into user values ('admin', 'all',0);");
		nRows = m_DB.execDML("insert into user values ('admin_pc', 'all',0);");
		nRows = m_DB.execDML("insert into user values ('admin_android', 'all',0);");

		/*nRows = m_DB.execDML("insert into user values ('administrator', 'admin',99);");
		nRows = m_DB.execDML("update user set priority = 99 where username = 'admin';");
		nRows = m_DB.execDML("delete from user where username = 'administrator';");*/
	}
	catch (CppSQLite3Exception& e)
	{
		DebugLog("%s", e.errorMessage());
		return false;
	}

	return true;
}

bool DBManager::VerifyUser(std::string username, std::string pwd)
{
	CppSQLite3Buffer bufSQL;
	bufSQL.format("select * from user where username = '%s'and userpwd = '%s';", username.c_str(), pwd.c_str());
	CppSQLite3Query q = m_DB.execQuery(bufSQL);
	if (q.eof())
	{
		return false;
	}
	return true;

}

std::vector<std::string> DBManager::GetUserRobotList(std::string username)
{
	std::vector<std::string> list;

	CppSQLite3Buffer bufSQL;
	bufSQL.format("select * from user2robot where username = '%s';", username.c_str());
	CppSQLite3Query q = m_DB.execQuery(bufSQL);
	if (q.eof())
	{
	}
	else
	{
		/*q.
		std::string priority = q.getStringField("udid");
		return priority;*/
	}

	return list;
}
