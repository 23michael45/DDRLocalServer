#include "StreamServiceInfoProcessor.h"
#include <memory>
#include "../../../Shared/proto/BaseCmd.pb.h"
#include "../../../Shared/src/Utility/DDRMacro.h"

#include "../../../Shared/src/Utility/Logger.h"
#include "../../Managers/GlobalManager.h"
#include "../../Managers/StreamRelayServiceManager.h"
using namespace DDRFramework;
using namespace DDRCommProto;

StreamServiceInfoProcessor::StreamServiceInfoProcessor(BaseMessageDispatcher& dispatcher) :BaseProcessor(dispatcher)
{ 
}


StreamServiceInfoProcessor::~StreamServiceInfoProcessor()
{
}

void StreamServiceInfoProcessor::Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{

	reqStreamServiceInfo* pRaw = reinterpret_cast<reqStreamServiceInfo*>(spMsg.get());


	auto sprsp = std::make_shared<rspStreamServiceInfo>();

	//sprsp->set_srcip("rtsp://192.168.1.88");
	for (auto channel : StreamRelayServiceManager::Instance()->GetAVChannels())
	{
			auto pchannel = sprsp->add_channels();
			pchannel->CopyFrom(channel);
	}
	sprsp->set_tcpport(StreamRelayServiceManager::Instance()->GetServerTcpPort());

	spSockContainer->Send(sprsp);

}