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


int main()
{
	//auto spClient = std::make_shared<TcpClientBase>();
	auto spClient = std::make_shared<LocalTcpClient>();
	spClient->Start("127.0.0.1","88");

	std::thread t = std::thread(std::bind([](std::shared_ptr<TcpClientBase> spClient) {

		
		for (int i = 0 ; i<1000000 ; i++)
		{

			//std::this_thread::sleep_for(chrono::seconds(1));

			reqLogin req;
			req.set_username("michael");

			if (spClient && spClient->IsConnected())
			{
				spClient->Send(req);
			}
			else
			{
			}
			//DebugLog("\nUse Count:%i", spTcpClientBase.use_count());
		}

	}, spClient));
	t.detach();

	while (gQuit != 'q')
	{
		gQuit = getchar();
	}
	return 0;
}