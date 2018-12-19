#include "VideoStreamInfoProcessor.h"
#include <memory>
#include "../../../Shared/proto/BaseCmd.pb.h"
#include "../../../Shared/src/Utility/DDRMacro.h"

#include "../../../Shared/src/Utility/Logger.h"
using namespace DDRFramework;
using namespace DDRCommProto;

VideoStreamInfoProcessor::VideoStreamInfoProcessor(BaseMessageDispatcher& dispatcher) :BaseProcessor(dispatcher)
{ 
}


VideoStreamInfoProcessor::~VideoStreamInfoProcessor()
{
}

void VideoStreamInfoProcessor::Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{

	reqVideoStreamInfo* pRaw = reinterpret_cast<reqVideoStreamInfo*>(spMsg.get());


	auto sprsp = std::make_shared<rspVideoStreamInfo>();

	//sprsp->set_srcip("rtsp://192.168.1.88");
	auto pchannel = sprsp->add_channels();

	pchannel->set_srcip("rtsp://192.168.1.100/11");
	pchannel->set_dstip("rtmp://193.112.189.182:1935/live/test1");

	spSockContainer->Send(sprsp);

}