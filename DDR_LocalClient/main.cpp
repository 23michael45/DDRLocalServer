#include "../../Shared/src/Network/MessageSerializer.h"
#include "../../Shared/src/Network/TcpServerBase.h"
#include "../../Shared/src/Network/TcpClientBase.h"
#include "../../Shared/src/Network/HttpClient.h"
#include "../../Shared/proto/BaseCmd.pb.h"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include "../../Shared/src/Utility/DDRMacro.h"

#include "../../Shared/src/Utility/MiniDump.h"
#include "Client/LocalClientUdpDispatcher.h"
#include <thread>
#include <chrono>
#include "Client/LocalTcpClient.h"
#include "Client/GlobalManager.h"
#include "Client/AudioTcpClient.h"
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

#include "../../../Shared/src/Utility/CommonFunc.h"
#ifdef _WINDOWS
#include <Windows.h>
#include "cppfs/windows/LocalFileSystem.h"
#endif


#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"

#include "../../Shared/src/Utility/PythonDebugTools.h"

using namespace DDRFramework;
using namespace DDRCommProto;

using namespace std;
char gQuit = 0;


void DoOnce(std::shared_ptr<TcpClientBase> spClient)
{

	std::this_thread::sleep_for(chrono::seconds(2));
	auto spSession = spClient->Connect("127.0.0.1", "88");

	std::thread t = std::thread(std::bind([](std::shared_ptr<TcpClientBase> spClient) {


		for (int j = 0; j < 2;)
		{
			try
			{

				for (int i = 0; i < 10000;)
				{

					//std::this_thread::sleep_for(chrono::seconds(1));

					auto spreq = std::make_shared<reqLogin>();
					spreq->set_username("michaelmichaelmichaelmichaelmichaelmichaelmichaelmichaelmichaelmichael");

					const char* psrc = "1234567890";

					if (spClient && spClient->IsConnected())
					{
						//spClient->Send(spreq);
						spClient->Send(psrc, 10);
						i++;
					}
					spreq.reset();
				}
			}
			catch (asio::system_error& e)
			{
				break;
			}


			std::this_thread::sleep_for(chrono::seconds(10));
		}

	}, spClient));
	t.join();

	getchar();

	std::this_thread::sleep_for(chrono::seconds(2));
	spClient->Disconnect(spSession);
	spClient.reset();
}

std::shared_ptr<AudioTcpClient> TcpAudioClient()
{
	//std::shared_ptr<AudioTcpClient> spAudioClient = std::make_shared<AudioTcpClient>();
	auto spAudioClient = std::shared_ptr<AudioTcpClient>(new AudioTcpClient);
	//spAudioClient->Connect("192.168.1.183", "88");
	spAudioClient->Start();
	spAudioClient->Connect("192.168.1.183", "89");

	return spAudioClient;
}


class _ConsoleDebug : public DDRFramework::ConsoleDebug, public CSingleton<_ConsoleDebug>
{
public:
	_ConsoleDebug()
	{
		AddCommand("ls cc", std::bind(&_ConsoleDebug::ListClientConnection, this));
		AddCommand("castart", std::bind(&_ConsoleDebug::StartAudio, this));
		AddCommand("castop", std::bind(&_ConsoleDebug::StopAudio, this));

		AddCommand("reqf", std::bind(&_ConsoleDebug::RequestFile, this));
		AddCommand("reqrf", std::bind(&_ConsoleDebug::RequestRemoteFile, this));
		AddCommand("upf", std::bind(&_ConsoleDebug::UploadFile, this));


		AddCommand("connect", std::bind(&_ConsoleDebug::Connect, this));
		AddCommand("httpget", std::bind(&_ConsoleDebug::HttpGet, this));
		AddCommand("cmd", std::bind(&_ConsoleDebug::SendCmd, this));

		AddCommand("cmdmove", std::bind(&_ConsoleDebug::SendCmdMove, this));
		AddCommand("slist", std::bind(&_ConsoleDebug::GetServerList, this));

		AddCommand("rlogin", std::bind(&_ConsoleDebug::RemoteLogin, this));
		AddCommand("sls", std::bind(&_ConsoleDebug::SelectLS, this));

		AddCommand("py", std::bind(&_ConsoleDebug::RunPython, this));
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

	void StartAudio()
	{
		DebugLog("Call Audio");
		auto spreq = std::make_shared<reqStreamAddr>();
		spreq->set_networktype(ChannelNetworkType::Local);
		
		GlobalManager::Instance()->GetTcpClient()->Send(spreq);
		spreq.reset();
	}
	void StopAudio()
	{

		GlobalManager::Instance()->StopAudioClient();
	}
	void RequestFile()
	{
		DebugLog("Request File");
		auto spreq = std::make_shared<reqFileAddress>();
		spreq->set_tarservicetype(eCltType::eLSMStreamRelay);
		spreq->set_filetype(eFileTypes::FileHttpAddress);
		//spreq->add_filenames("x64/*DDR*Server.e*");
		//spreq->add_filenames("x64/*api*l*.*");
		//spreq->add_filenames("*中*/*件*.*");
		spreq->add_filenames("x64/Debug/*.db");
		spreq->add_filenames("fs/*/*.*");


		GlobalManager::Instance()->GetTcpClient()->Send(spreq);
		spreq.reset();
	}
	void RequestRemoteFile()
	{
		DebugLog("Request File");
		auto spreq = std::make_shared<reqRemoteFileAddress>();
		spreq->set_tarservicetype(eCltType::eLSMStreamRelay);
		spreq->set_filetype(eFileTypes::FileHttpAddress);
		//spreq->add_filenames("x64/*DDR*Server.e*");
		//spreq->add_filenames("x64/*api*l*.*");
		//spreq->add_filenames("*中*/*件*.*");
		//spreq->add_filenames("x64/Debug/*.db");
		spreq->add_filenames("fs/*/*.*");


		GlobalManager::Instance()->GetTcpClient()->Send(spreq);
		spreq.reset();
	}

	void UploadFile()
	{
		auto spHttpSession = std::make_shared<HttpSession>();

		std::string url = "http://localhost:8000/";
		std::string basedir = "D:/DevelopProj/Dadao/DDRFramework/www/";
		std::string filename = "fs/root/t.dll";
		spHttpSession->DoPost(url, basedir , filename);


		//std::this_thread::sleep_for(chrono::seconds(10));
	}

	void GetServerList()
	{
		GlobalManager::Instance()->ConnectBroadcastServer();
	}
	void SelectLS()
	{
		rspRemoteLogin& info = GlobalManager::Instance()->GetRemoteLoginInfo();

		if (info.lslist().size() > 0)
		{
			auto ls = info.lslist(0);
			
			auto spreq = std::make_shared<reqSelectLS>();
			
			spreq->set_udid(ls.udid());

			GlobalManager::Instance()->GetTcpClient()->Send(spreq);
		}
	}
	void RemoteLogin()
	{
		auto spreq = std::make_shared<reqRemoteLogin>();

		spreq->set_type(eCltType::eRemotePCClient);
		spreq->set_username("admin");
		spreq->set_userpwd("admin");

		GlobalManager::Instance()->GetTcpClient()->Send(spreq);
		DebugLog("Send Romote Login");

	}


	void Connect()
	{
		auto vec = split(m_CurrentCmd, ':');
		if (vec.size() == 3)
		{
			GlobalManager::Instance()->StopUdp();
			GlobalManager::Instance()->TcpConnect(vec[1],vec[2]);
		}
		else if(vec.size() == 1)
		{
			GlobalManager::Instance()->StopUdp();
			GlobalManager::Instance()->TcpConnect();

		}

	}
	void HttpGet()
	{

		auto spHttpSession = std::make_shared<HttpSession>();
		//auto url = std::string("http://192.168.1.183:8080////bin/x64/26-12-2018-17-00-4.log");
		//auto outfile = std::string("D:/DevelopProj/Dadao/DDRFramework/DDRLocalServer/x64/Debug/26-12-2018-17-00-4.log");
		auto url = std::string("http://192.168.1.183:8080/wav/9.wav");
		//auto url = std::string("http://192.168.1.183:8080/wav/wav.wav");
		//auto url = std::string("http://192.168.1.183:8080/wav/a.txt");
		auto outfile = std::string("D:/DevelopProj/Dadao/DDRFramework/DDRLocalServer/x64/Debug/x.wav");
		spHttpSession->DoGet(url ,outfile);
	}

	void OpenCVTest()
	{
		cv::Mat mat;
		int cols = mat.cols;
	}

	void SendCmd()
	{
		auto spreq = std::make_shared<reqCmdMove>();
		spreq->set_line_speed(100);
		spreq->set_angulau_speed(200);


		auto spheader = std::make_shared<CommonHeader>();
		GlobalManager::Instance()->GetTcpClient()->Send(spheader,spreq);
		spreq.reset();
	}

	void SendCmdMove()
	{
		auto spreq = std::make_shared<reqCmdMove>();
		spreq->set_line_speed(100);
		spreq->set_angulau_speed(200);


		GlobalManager::Instance()->GetTcpClient()->Send(spreq);
		spreq.reset();
	}
	void RunPython()
	{
		auto vec = split(m_CurrentCmd, ':');

		std::string funcname = "msg";
		if (vec.size() == 2)
		{
			funcname = vec[1];
		}
		
		PythonDebugTools pdt(GlobalManager::Instance()->GetGlobalConfig().GetValue("PythonPath"));
		auto spmsg = pdt.Run(funcname);
		if (spmsg)
		{
			GlobalManager::Instance()->GetTcpClient()->Send(spmsg);

		}
		else
		{

			DebugLog("RunPython Error");
		}
	}

};

int main()
{
	DisableMouseSelectConsole();

	InitMinDump();

	GlobalManager::Instance()->Init();

	
	_ConsoleDebug::Instance()->ConsoleDebugLoop();
	return 0;
}