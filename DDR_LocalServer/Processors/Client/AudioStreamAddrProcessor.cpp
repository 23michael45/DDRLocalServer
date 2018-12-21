#include "AudioStreamAddrProcessor.h"
#include "../../../Shared/proto/BaseCmd.pb.h"
#include "../../../Shared/src/Utility/DDRMacro.h"
using namespace DDRFramework;
using namespace DDRCommProto;


AudioStreamAddrProcessor::AudioStreamAddrProcessor(BaseMessageDispatcher& dispatcher) : BaseProcessor::BaseProcessor(dispatcher)
{
}


AudioStreamAddrProcessor::~AudioStreamAddrProcessor()
{
}


void AudioStreamAddrProcessor::Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{
	auto bodytype = spHeader->bodytype();
	reqAudioStreamAddr* pRaw = reinterpret_cast<reqAudioStreamAddr*>(spMsg.get());


	auto sprsp = std::make_shared<rspAudioStreamAddr>();

	auto *pChannel = sprsp->add_channels();
	pChannel->set_dstip("192.168.1.183");
	pChannel->set_dstport(89);
	pChannel->set_type(AVChannel_ChannelType_Local);

	spSockContainer->Send(sprsp);
}
