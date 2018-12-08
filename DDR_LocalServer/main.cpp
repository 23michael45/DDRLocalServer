#include "../../Shared/src/Network/MessageSerializer.h"
#include "../../Shared/src/Network/TcpServerBase.h"
#include "../../Shared/src/Network/TcpClientBase.h"
#include "LocalTcpServer.h"

using namespace DDRFramework;
int main()
{

	std::make_shared<LocalTcpServer>(88)->Start();
	//std::make_shared<TcpServerBase>(88)->Start();



	char q = getchar();
	return 0;
}