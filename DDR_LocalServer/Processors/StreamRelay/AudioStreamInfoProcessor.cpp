#include "AudioStreamInfoProcessor.h"
#include <memory>
#include "../../../Shared/proto/BaseCmd.pb.h"
#include "../../../Shared/src/Utility/DDRMacro.h"

#include "../../../Shared/src/Utility/Logger.h"
using namespace DDRFramework;
using namespace DDRCommProto;

AudioStreamInfoProcessor::AudioStreamInfoProcessor(BaseMessageDispatcher& dispatcher) :BaseProcessor(dispatcher)
{ 
}


AudioStreamInfoProcessor::~AudioStreamInfoProcessor()
{
}

void AudioStreamInfoProcessor::Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{

	reqAudioStreamInfo* pRaw = reinterpret_cast<reqAudioStreamInfo*>(spMsg.get());


	auto sprsp = std::make_shared<rspAudioStreamInfo>();




	spSockContainer->Send(sprsp);
}