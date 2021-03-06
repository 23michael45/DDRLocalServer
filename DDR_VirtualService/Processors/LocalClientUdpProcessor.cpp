#include "LocalClientUdpProcessor.h"
#include <memory>
#include "src/Utility/DDRMacro.h"
#include "src/Utility/CommonFunc.h"
#include "src/Network/HttpServer.h"
#include "thirdparty/asio/include/asio.hpp"
#include "../Client/GlobalManager.h"
#include "../Client/HttpFileServer.h"

using namespace DDRFramework;
using namespace DDRCommProto;

LocalClientUdpProcessor::LocalClientUdpProcessor(BaseMessageDispatcher& dispatcher) :BaseProcessor(dispatcher)
{
}


LocalClientUdpProcessor::~LocalClientUdpProcessor()
{
	DebugLog("LocalClientUdpProcessor Destroy");
}

void LocalClientUdpProcessor::Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{
	auto bodytype = spHeader->bodytype();

	bcLSAddr* pRaw = reinterpret_cast<bcLSAddr*>(spMsg.get());



	for (auto serverinfo : pRaw->lsinfos())
	{
		if (serverinfo.robotid() == GlobalManager::Instance()->GetRobotID())
		{


			if (serverinfo.stype() == bcLSAddr_eServiceType_LocalServer)
			{

				DealLocalServer(serverinfo);
			}
			else if (serverinfo.stype() == bcLSAddr_eServiceType_RemoteServer)
			{
			}
			else if (serverinfo.stype() == bcLSAddr_eServiceType_RTSPStreamServer)
			{
			}
			else if (serverinfo.stype() == bcLSAddr_eServiceType_TalkBackServer)
			{
			}
		}
		else
		{
			//Not Same Robot , Abandon
		}
	}
	



}
void LocalClientUdpProcessor::DealLocalServer(bcLSAddr_ServerInfo& serverinfo)
{
	std::string name = serverinfo.name();
	std::string ips;
	int port = serverinfo.port();


	auto localAddr = DDRFramework::GetLocalIPV4();


	std::vector<std::string> remoteAddrs;
	for (auto ip : serverinfo.ips())
	{
		remoteAddrs.push_back(ip);
	}

	auto rmap = DDRFramework::GetSameSegmentIPV4(localAddr, remoteAddrs);
	if (rmap.size() > 0)
	{
		auto conntectip = (rmap.begin())->second;

		TcpClientStart(conntectip, port);


		//HttpServerStart((rmap.begin())->first, GlobalManager::Instance()->GetConfig().GetValue("HttpPort"), GlobalManager::Instance()->GetConfig().GetValue("HttpRoot"));

		DebugLog("Receive Server Broadcast %s: %s", name.c_str(), conntectip.c_str());
	}
	else
	{



		DebugLog("Receive Server Broadcast But No IP in same segment");
	}

}


void LocalClientUdpProcessor::TcpClientStart(std::string serverip, int serverport)
{
	GlobalManager::Instance()->StopUdp();
	std::ostringstream strport;
	strport << serverport;
	const std::string sPort(strport.str());
	GlobalManager::Instance()->TcpConnect(serverip, sPort);


}

void LocalClientUdpProcessor::HttpServerStart(const std::string& serverip, std::string& serverport,std::string& docroot)
{
	HttpFileServer::Instance()->Start(serverip,serverport,docroot);
}
