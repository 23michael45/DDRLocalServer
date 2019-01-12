#include "../../../Shared/src/Network/MessageSerializer.h"
#include "../../../Shared/src/Network/TcpServerBase.h"
#include "../../../Shared/src/Network/TcpClientBase.h"
#include "../../../Shared/src/Utility/XmlLoader.h"
#include "../../../Shared/src/Utility/LocalizationLoader.h"
#include "../../../Shared/src/Utility/MiniDump.h"
#include "../../../Shared/src/Utility/CommonFunc.h"
#include "../../../Shared/src/Network/UdpSocketBase.h"
#include "Servers/LocalServerUdpDispatcher.h"
#include "Managers/GlobalManager.h"
#include "Managers/FileManager.h"
#include "Servers/LocalTcpServer.h"
#include "LSClient/LSClientManager.h"

#include "../../Shared/src/Utility/PythonDebugTools.h"

#include <thread>
#include <chrono>
using namespace DDRFramework;
using namespace DDRCommProto;
using namespace std;

#ifdef _WINDOWS
#include <Windows.h>
#include <cppfs/windows/LocalFileSystem.h>
#endif
#include <cppfs/FilePath.h>
#include "Managers/FileManager.h"


char gQuit = 0;

#include "../../../Shared/thirdparty/curl-asio/curl-asio.hpp"
#include "../../../Shared/thirdparty/cpp-sqlite3/cppsqlite3.h"
#include <ctime>
#include <iostream>

#include <iostream>
#include <fstream>

#include "../../Shared/src/Utility/Logger.h"
#include <regex>


#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;


static curl_asio::data_action::type on_transfer_data_read(std::ofstream &out, const asio::const_buffer& buffer)
{
	out.write(asio::buffer_cast<const char*>(buffer), asio::buffer_size(buffer));
	return curl_asio::data_action::success;
}

static void on_transfer_done(curl_asio::transfer::ptr transfer, std::ofstream &out, const std::string &file, CURLcode result)
{
	if (result == CURLE_OK)
	{
		out.close();

		std::cout << "Transfer of " << transfer->info().effective_url() << " completed successfully (" << transfer->info().total_time() << " seconds)! Content saved to file " << file << std::endl;
		exit(0);
	}
	else
	{
		std::cerr << "Transfer of " << transfer->info().effective_url() << " failed with error " << result << std::endl;
		exit(1);
	}
}

int asiocurl(std::string filename,std::string usage,std::string url)
{
	asio::io_service io;
	curl_asio curl(io);
	curl_asio::transfer::ptr transfer = curl.create_transfer();
	if (transfer)
	{
		std::ofstream out(filename);
		transfer->opt.protocols = CURLPROTO_HTTP | CURLPROTO_HTTPS;
		transfer->opt.max_redirs = 5;
		transfer->opt.redir_protocols = CURLPROTO_HTTP | CURLPROTO_HTTPS;
		transfer->opt.follow_location = true;
		transfer->on_data_read = std::bind(on_transfer_data_read, std::ref(out), std::placeholders::_1);
		transfer->on_done = std::bind(on_transfer_done, transfer, std::ref(out), filename, std::placeholders::_1);
		if (transfer->start(url))
		{
			while (1)
				io.run();
		}
	}

	return 1;
}


class _ConsoleDebug : public DDRFramework::ConsoleDebug ,public CSingleton<_ConsoleDebug>
{
public:
	_ConsoleDebug()
	{
		AddCommand("ls sc", std::bind(&_ConsoleDebug::ListServerConnections, this));
		AddCommand("py", std::bind(&_ConsoleDebug::RunPython, this));
	}
	void ListServerConnections()
	{
		printf_s("\nServer Connections");
		for (auto pair : GlobalManager::Instance()->GetTcpServer()->GetTcpSocketContainerMap())
		{

			auto spSession = pair.second;
			auto spServerSessionTcp = dynamic_pointer_cast<LocalServerTcpSession>(spSession);

			std::string ip = spServerSessionTcp->GetSocket().remote_endpoint().address().to_string();
			printf_s("\n%s  type:%i", ip.c_str(), spServerSessionTcp->GetLoginInfo().type());
		}
	}
	void RunPython()
	{
		auto vec = split(m_CurrentCmd, ':');

		std::string funcname = "msg";
		std::string ip = "";
		if (vec.size() == 2)
		{
			funcname = vec[1];
		}
		else if (vec.size() == 3)
		{
			funcname = vec[1];
			ip = vec[2];
		}

		PythonDebugTools pdt(GlobalManager::Instance()->GetGlobalConfig().GetValue("PythonPath"));
		auto spmsg = pdt.Run(funcname);
		if (spmsg)
		{
			if (!ip.empty())
			{
				for (auto pair : GlobalManager::Instance()->GetTcpServer()->GetTcpSocketContainerMap())
				{
					auto pSocket = pair.first;

					if (pSocket)
					{
						std::string socketip = pSocket->remote_endpoint().address().to_string();
						if (socketip == ip)
						{

							auto spSession = pair.second;
							spSession->Send(spmsg);
						}
					}
				}

			}
			else
			{
				for (auto pair :GlobalManager::Instance()->GetTcpServer()->GetTcpSocketContainerMap())
				{
					auto spSession = pair.second;
					spSession->Send(spmsg);

				}

			}

		}
		else
		{

			DebugLog("RunPython Error");
		}
	}
};


int main()
{
	MsgRouterManager::Instance()->SetCltType(eLocalServer);
	DisableMouseSelectConsole();

	InitMinDump();
	GlobalManager::Instance()->StartTcpServer();
	GlobalManager::Instance()->StartUdpServer();

	LSClientManager::Instance()->Init();

	LSClientManager::Instance()->ConnectBroadcastServer();

	_ConsoleDebug::Instance()->ConsoleDebugLoop();

	return 0;
}