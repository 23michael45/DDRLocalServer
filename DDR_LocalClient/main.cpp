#include "../../Shared/src/Network/MessageSerializer.h"
#include "../../Shared/src/Network/TcpServerBase.h"
#include "../../Shared/src/Network/TcpClientBase.h"
#include "../../Shared/proto/BaseCmd.pb.h"
#include "../../Shared/src/Utility/DDRMacro.h"

#include <thread>
#include <chrono>
#include "LocalTcpClient.h"
using namespace DDRFramework;
using namespace DDRCommProto;

using namespace std;
char gQuit = 0;


void DoOnce()
{

	auto spClient = std::make_shared<TcpClientBase>();
	//auto spClient = std::make_shared<LocalTcpClient>();
	spClient->Start("127.0.0.1", "88");

	std::thread t = std::thread(std::bind([](std::shared_ptr<TcpClientBase> spClient) {


		for (int i = 0; i < 1000;)
		{

			//std::this_thread::sleep_for(chrono::seconds(1));

			reqLogin req;
			req.set_username("michaelmichaelmichaelmichaelmichael");

			if (spClient && spClient->IsConnected())
			{
				spClient->Send(req);
				i++;
			}
			
		}
		spClient->Stop();

		DebugLog("\nspClient Use Count:%i", spClient.use_count());

	}, spClient));
	t.detach();
	spClient.reset();
}

int main()
{
	for (int i = 0 ; i < 1000;i++)
	{
		DoOnce();
		std::this_thread::sleep_for(chrono::seconds(1));
	}

	while (true)
	{
		std::this_thread::sleep_for(chrono::seconds(1));
	}
	return 0;
}