#include "../../Shared/src/Network/MessageSerializer.h"
#include "../../Shared/src/Network/TcpServerBase.h"
#include "../../Shared/src/Network/TcpClientBase.h"
#include "../../Shared/src/Network/HttpClient.h"
#include "../../Shared/proto/BaseCmd.pb.h"
#include "../../Shared/src/Utility/DDRMacro.h"
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
#ifdef _WINDOWS
#include <Windows.h>
#include "cppfs/windows/LocalFileSystem.h"
#endif



#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"

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
void TcpClient()
{

	//auto spClient = std::make_shared<TcpClientBase>();
	auto spClient = std::make_shared<LocalTcpClient>();

	spClient->Start(1);
	for (int i = 0; i < 1; i++)
	{
		DoOnce(spClient);
		std::this_thread::sleep_for(chrono::seconds(1));
	}

	spClient->Stop();
}
void TcpUdpClient()
{

	GlobalManager::Instance()->StartTcpClient();
	GlobalManager::Instance()->GetTcpClient()->Start(4);

	GlobalManager::Instance()->StartUdp();


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

void LoopTestUdpMem()
{
	while (true)
	{
		TcpUdpClient();


		std::this_thread::sleep_for(chrono::seconds(2));

		if (GlobalManager::Instance()->IsUdpWorking())
		{
			GlobalManager::Instance()->GetUdpClient()->StopReceive();
			GlobalManager::Instance()->GetUdpClient()->Stop();

		}
	}
}


class _ConsoleDebug : public DDRFramework::ConsoleDebug, public CSingleton<_ConsoleDebug>
{
public:
	_ConsoleDebug()
	{
		AddCommand("ls cc", std::bind(&_ConsoleDebug::ListClientConnection, this));
		AddCommand("ca", std::bind(&_ConsoleDebug::CallAudio, this));
		AddCommand("reqf", std::bind(&_ConsoleDebug::RequestFile, this));
		AddCommand("connect", std::bind(&_ConsoleDebug::Connect, this));
		AddCommand("httpget", std::bind(&_ConsoleDebug::HttpGet, this));
		AddCommand("cmd", std::bind(&_ConsoleDebug::SendCmd, this));
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

	void CallAudio()
	{
		DebugLog("Call Audio");
		auto spreq = std::make_shared<reqStreamAddr>();
		spreq->set_networktype(ChannelNetworkType::Local);
		
		GlobalManager::Instance()->GetTcpClient()->Send(spreq);
		spreq.reset();
	}

	void RequestFile()
	{
		DebugLog("Request File");
		auto spreq = std::make_shared<reqFileAddress>();
		spreq->set_tarservicetype(eCltType::eLSMStreamRelay);
		spreq->set_filetype(eFileTypes::FileHttpAddress);
		//spreq->add_filenames("x64/*DDR*Server.e*");
		spreq->add_filenames("x64/*api*l*.*");
		//spreq->add_filenames("*жа*/*Мў*.*");
		spreq->add_filenames("*.wav");


		GlobalManager::Instance()->GetTcpClient()->Send(spreq);
		spreq.reset();
	}


	void Connect()
	{
		auto vec = split(m_CurrentCmd, ':');
		if (vec.size() == 3)
		{

			GlobalManager::Instance()->TryConnect(vec[1],vec[2]);
		}
		else if(vec.size() == 1)
		{
			GlobalManager::Instance()->TryConnect();

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
};

int main()
{
#ifdef _WINDOWS
	SetConsoleOutputCP(CP_UTF8);
#endif
	TcpUdpClient();
	
	_ConsoleDebug::Instance()->ConsoleDebugLoop();
	return 0;
}