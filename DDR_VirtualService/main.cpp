#include<iostream>
#include "../../Shared/src/Network/MessageSerializer.h"
#include "../../Shared/src/Network/TcpServerBase.h"
#include "../../Shared/src/Network/TcpClientBase.h"
#include "../../Shared/src/Network/HttpServer.h"
#include "../../Shared/proto/BaseCmd.pb.h"
#include "../../Shared/src/Utility/DDRMacro.h"
#include "../../Shared/src/Utility/CommonFunc.h"
#include "../../Shared/src/Network/HttpClient.h"
#include "Client/LocalClientUdpDispatcher.h"
#include <thread>
#include <chrono>
#include <fstream>
#include "Client/GlobalManager.h"
#include <functional>
#include <regex>
#include "../../../Shared/thirdparty/cppfs/include/cppfs/windows/LocalFileSystem.h"

#include "../../Shared/src/Utility/AudioCodec.h"
#include "Client/HttpFileServer.h"

#include "../../Shared/src/Utility/MiniDump.h"
#include "Client/FileManager.h"
#include "../../Shared/proto/SimulationCmd.pb.h"


using namespace DDRFramework;
using namespace DDRCommProto;

class _ConsoleDebug : public DDRFramework::ConsoleDebug, public CSingleton<_ConsoleDebug>
{
public:
	_ConsoleDebug()
	{
		AddCommand("ls cc", std::bind(&_ConsoleDebug::ListClientConnection, this));



		AddCommand("notify", std::bind(&_ConsoleDebug::NotifyAlarm, this));
		AddCommand("checkfiles", std::bind(&_ConsoleDebug::CheckFiles, this));


		AddCommand("startsend", std::bind(&_ConsoleDebug::StartSend, this));
		AddCommand("stopsend", std::bind(&_ConsoleDebug::StopSend, this));


		AddCommand("basestatus", std::bind(&_ConsoleDebug::SendBaseStatus, this));

		AddCommand("diffdrive", std::bind(&_ConsoleDebug::SendDiffrentialDirveCmd, this));

	}
	
	void ListClientConnection()
	{
		printf_s("\nClient Connection");
		auto spSession = GlobalManager::Instance()->GetTcpClient()->GetConnectedSession();
		if (spSession)
		{
			std::string ip = spSession->GetSocket().remote_endpoint().address().to_string();
			printf_s("\n%s", ip.c_str());
		}
		else
		{
			printf_s("\nClient Not Connection");
		}
	}
	

	void NotifyAlarm()
	{
		printf_s("\nSend Alarm");
		auto spSession = GlobalManager::Instance()->GetTcpClient()->GetConnectedSession();
		if (spSession)
		{

			auto spreq = std::make_shared<notifyBaseStatus>();
			spreq->set_batt(0.1f);
			spreq->set_currpath("12");
			spSession->Send(spreq);
			spreq.reset();
		}
		else
		{
			printf_s("\nClient Not Connection");
		}
	}


	void SendBaseStatus()
	{

		auto spreq = std::make_shared<notifyBaseStatus>();
		spreq->set_mode(1);
		spreq->set_sonmode(1);
		spreq->set_batt(1 * 0.01f);
		spreq->set_temp(0xffff);
		spreq->set_hum(10 * 0.01f);

		spreq->set_pm25(1);


		spreq->set_stopstat(10);
		spreq->set_posx(0.0f);
		spreq->set_posy(0.0f);
		spreq->set_posdirection(0.0f);
		spreq->set_poslinespeed(5);
		spreq->set_posangulauspeed(0.0f);
		spreq->set_longitude(10);
		spreq->set_latitude(10);
		spreq->set_timestamp(10);

		spreq->set_funcstat(10);

		spreq->set_currroute("PathError");
		spreq->set_currpath("Path_xxx.txt");
		spreq->set_currspeed(0.0f);

		spreq->set_currwalkstyle("ws");
		spreq->set_voiceloop(0);
		spreq->set_thermalmaxx(1);
		spreq->set_thermalmaxy(1);
		spreq->set_thermalmaxvalue(1);

		auto spSession = GlobalManager::Instance()->GetTcpClient()->GetConnectedSession();
		if (spSession)
		{
			while (true)
			{

				spSession->Send(spreq);

				std::this_thread::sleep_for(chrono::milliseconds(10));
			}
		}
		spreq.reset();
	}

	DDRFramework::Timer m_Timer;
	DDRFramework::timer_id m_HeartBeatTimerID;
	void StartSend()
	{
		m_HeartBeatTimerID = m_Timer.add(std::chrono::milliseconds(50), std::bind(&_ConsoleDebug::SendOnce, this, std::placeholders::_1), std::chrono::milliseconds(1));
	}
	void SendOnce(timer_id id)
	{
		printf_s("\nSend Alarm");
		auto spSession = GlobalManager::Instance()->GetTcpClient()->GetConnectedSession();
		if (spSession)
		{

			auto spreq = std::make_shared<notifyBaseStatus>();
			//spreq->set_type(eCltType::eLSMVirtualService);
			spreq->set_batt(0.1f);
			//spreq->set_whatever("0");
			spSession->Send(spreq);
			spreq.reset();
		}
		else
		{
			printf_s("\nClient Not Connection");
		}
	}
	void StopSend()
	{

		m_Timer.remove(m_HeartBeatTimerID);
	}

	void CheckFiles() {

		std::string fmt = "OneRoute*";
		//auto matchedFiles = FileManager::Instance()->MatchDir("OneRouteXXX", fmt);
		//std::vector<std::string> v;
		//for (auto dir : matchedFiles)
		//{
		//	auto finalFiles = FileManager::Instance()->MatchDir(dir, "bkPic.png");
		//	for (auto f : finalFiles)
		//	{
		//		v.push_back(f);
		//	}
		//}

		//auto listfiles = FileManager::Instance()->CheckFiles();

		//std::string fmt = "roo*\\*\\*.txt";
		//std::string fmt = "x*/Debug/*.db";
		//std::string fmt = "fs/*.txt";
		fmt = "OneRoute_111\\bkPic.png";
		//fmt = "bkPic.png";
		auto vec = FileManager::Instance()->Match(fmt);
		DebugLog("%s",vec[1].c_str());
		//for (auto s : vec)
		//{
		//	DebugLog("%s", s.c_str());
		//}

		//auto sprsp = std::make_shared<rspFileAddress>();


		//auto files = FileManager::Instance()->Match(fmt);

		//for (auto file : files)
		//{


		//	std::string httpaddr = HttpFileServer::Instance()->GetHttpFullPath(file);





		//	sprsp->add_fileaddrlist(httpaddr);



		//	auto spHttpSession = std::make_shared<HttpSession>();


		//	cppfs::FilePath fpath(httpaddr);
		//	auto filename = fpath.fileName();
		//	spHttpSession->DoGet(httpaddr, filename);



		//}



		//GlobalManager::Instance()->GetTcpClient()->Send(sprsp);

	}
	void SendDiffrentialDirveCmd()
	{
		auto spServer = GlobalManager::Instance()->GetTcpServer();
		if (spServer)
		{
			auto spnotify = std::make_shared<notifyDifferentialDrive>();

			spnotify->set_leftrpm(100);
			spnotify->set_rightrpm(100);

			auto sessoins = spServer->GetConnectedSessions();
			if (sessoins.size() > 0)
			{
				sessoins[0]->Send(spnotify);

			}

		}
	}

};



int main(int argc, char **argv)
{
	DisableMouseSelectConsole();
	InitMinDump();


	//HttpFileServer::Instance()->Init();
	GlobalManager::Instance()->Init();


	_ConsoleDebug::Instance()->ConsoleDebugLoop();
	return 0;
}

