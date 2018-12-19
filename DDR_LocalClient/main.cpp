#include "../../Shared/src/Network/MessageSerializer.h"
#include "../../Shared/src/Network/TcpServerBase.h"
#include "../../Shared/src/Network/TcpClientBase.h"
#include "../../Shared/proto/BaseCmd.pb.h"
#include "../../Shared/src/Utility/DDRMacro.h"
#include "Client/LocalClientUdpDispatcher.h"
#include <thread>
#include <chrono>
#include "Client/LocalTcpClient.h"
#include "Client/GlobalManager.h"
using namespace DDRFramework;
using namespace DDRCommProto;

using namespace std;
char gQuit = 0;


void DoOnce(std::shared_ptr<TcpClientBase> spClient)
{

	std::this_thread::sleep_for(chrono::seconds(2));
	spClient->Connect("127.0.0.1", "88");

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

					if (spClient && spClient->IsConnected())
					{
						spClient->Send(spreq);
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
	spClient->Disconnect();
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
void UdpClient()
{

	GlobalManager::Instance()->CreateUdp();
	GlobalManager::Instance()->GetUdpClient()->Start();
	GlobalManager::Instance()->GetUdpClient()->GetSerializer()->BindDispatcher(std::make_shared<LocalClientUdpDispatcher>());
	GlobalManager::Instance()->GetUdpClient()->StartReceive(28888);


}


int main()
{
	UdpClient();

	while (!gQuit)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	return 0;
}