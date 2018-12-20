#include "VideoStreamServiceInfoProcessor.h"
#include <memory>
#include "../../../Shared/proto/BaseCmd.pb.h"
#include "../../../Shared/src/Utility/DDRMacro.h"

#include "../../../Shared/src/Utility/Logger.h"
using namespace DDRFramework;
using namespace DDRCommProto;

VideoStreamServiceInfoProcessor::VideoStreamServiceInfoProcessor(BaseMessageDispatcher& dispatcher) :BaseProcessor(dispatcher)
{ 
}


VideoStreamServiceInfoProcessor::~VideoStreamServiceInfoProcessor()
{
}

void VideoStreamServiceInfoProcessor::Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{

	reqVideoStreamServiceInfo* pRaw = reinterpret_cast<reqVideoStreamServiceInfo*>(spMsg.get());


	auto sprsp = std::make_shared<rspVideoStreamServiceInfo>();

	//sprsp->set_srcip("rtsp://192.168.1.88");
	auto pchannel = sprsp->add_channels();

	pchannel->set_srcip("rtsp://192.168.1.100/11");
	pchannel->set_dstip("rtmp://193.112.189.182:1935/live/test1");

	spSockContainer->Send(sprsp);

}