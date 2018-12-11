#include "LocalClientUdpProcessor.h"
#include <memory>
#include "../../Shared/proto/BaseCmd.pb.h"
#include "../../Shared/src/Utility/DDRMacro.h"
using namespace DDRFramework;
using namespace DDRCommProto;

LocalClientUdpProcessor::LocalClientUdpProcessor()
{
}


LocalClientUdpProcessor::~LocalClientUdpProcessor()
{
}

void LocalClientUdpProcessor::Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{
	auto bodytype = spHeader->bodytype();

	broadcastServerInformation* pRaw = reinterpret_cast<broadcastServerInformation*>(spMsg.get());

	std::string name = pRaw->servername();
	std::string ips;
	for (auto ip : pRaw->ips())
	{
		ips += ":" + ip;
	}

	DebugLog("\nReceive Server Broadcast %s: %s" ,name.c_str(),ips.c_str());


}
void LocalClientUdpProcessor::AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{

}