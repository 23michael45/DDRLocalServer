#include "../../Shared/src/Network/MessageSerializer.h"
#include "../../Shared/src/Network/TcpServerBase.h"
#include "../../Shared/src/Network/TcpClientBase.h"
#include "LocalTcpServer.h"
#include "../../Shared/src/Utility/XmlLoader.h"
#include "../../Shared/src/Network/UdpSocketBase.h"
#include "LocalServerUdpDispatcher.h"

#include <thread>
#include <chrono>
using namespace DDRFramework;
using namespace DDRCommProto;
using namespace std;


void UdpServer()
{
	auto spBroadcast = std::make_shared<broadcastServerInformation>();
	spBroadcast->set_servername("ServerName");
	spBroadcast->set_port("28888");

	asio::io_service io_service;
	tcp::resolver resolver(io_service);
	tcp::resolver::query query(asio::ip::host_name(), "");
	tcp::resolver::iterator iter = resolver.resolve(query);
	tcp::resolver::iterator end; // End marker.


	while (iter != end)
	{
		tcp::endpoint ep = *iter++;

		auto p = spBroadcast->add_ips();
		*p = ep.address().to_string();
	}

	auto spUdp = std::make_shared<UdpSocketBase>();

	spUdp->Start();
	spUdp->GetSerializer()->BindDispatcher(std::make_shared<LocalServerUdpDispatcher>());

	spUdp->StartBroadcast(28888, spBroadcast, 2000);
	//spUdp->StartReceive(28888);


	std::this_thread::sleep_for(chrono::seconds(1000));

	//spUdp->StopReceive();
	spUdp->StopBroadcast();
	spUdp->Stop();
	spUdp.reset();
}

void TcpServer()
{
	XmlLoader loader("Config/LocalServerConfig.xml");
	std::string port = loader.GetValue("Port");
	std::string servername = loader.GetValue("ServerName");
	std::string threadCount = loader.GetValue("ThreadCount");

	//loader.SetValue(std::string("ServerName"), std::string("LocalServerV2"));
	//loader.DoSave();

	std::make_shared<LocalTcpServer>(std::stoi(port))->Start(std::stoi(threadCount));
	std::make_shared<TcpServerBase>(88)->Start();



}
void TimerHandle(const asio::error_code& ec)
{
	DebugLog("\nTimer Handle");
}
void TimerHandleStop(const asio::error_code& ec)
{
	DebugLog("\nTimer Handle");
}
void SteadyTimer()
{
	asio::io_service ioservice1;
	asio::io_service ioservice2;

	asio::steady_timer timer1{ ioservice1, std::chrono::seconds{5} };
	timer1.async_wait([](const asio::error_code &ec)
	{ 
		if (!ec)
		{
			std::cout << "3 sec\n";
		}
		else
		{

			std::cout << "timer error\n";

		}
	});

	std::shared_ptr<asio::steady_timer> timer2 = std::make_shared<asio::steady_timer>( ioservice2, std::chrono::seconds{5} );


	timer2->async_wait([](const asio::error_code &ec)
	{
		if (!ec)
		{
			std::cout << "3 sec\n";
		}
		else
		{
			std::cout << "timer error\n";

		}
	});

	std::thread thread1{ [&ioservice1]() { ioservice1.run(); } };
	std::thread thread2{ [&ioservice2]() { ioservice2.run(); } };
	thread1.join();
	thread2.join();

}

int main()
{
	//SteadyTimer();



	TcpServer();
	//UdpServer();
	
	getchar();
	return 0;
}