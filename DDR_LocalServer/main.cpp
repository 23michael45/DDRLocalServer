#include "../../Shared/src/Network/MessageSerializer.h"
#include "../../Shared/src/Network/TcpServerBase.h"
#include "../../Shared/src/Network/TcpClientBase.h"
#include "LocalTcpServer.h"

#include <thread>
#include <chrono>
using namespace DDRFramework;
using namespace std;
int main()
{

	std::make_shared<LocalTcpServer>(88)->Start(4);
	//std::make_shared<TcpServerBase>(88)->Start();


	while (true)
	{

		std::this_thread::sleep_for(chrono::seconds(1));
	}
	return 0;
}