#include "AudioStreamServiceInfoProcessor.h"
#include <memory>
#include "../../../Shared/proto/BaseCmd.pb.h"
#include "../../../Shared/src/Utility/DDRMacro.h"

#include "../../../Shared/src/Utility/Logger.h"
using namespace DDRFramework;
using namespace DDRCommProto;

AudioStreamServiceInfoProcessor::AudioStreamServiceInfoProcessor(BaseMessageDispatcher& dispatcher) :BaseProcessor(dispatcher)
{ 
}


AudioStreamServiceInfoProcessor::~AudioStreamServiceInfoProcessor()
{
}

void AudioStreamServiceInfoProcessor::Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{

	reqAudioStreamServiceInfo* pRaw = reinterpret_cast<reqAudioStreamServiceInfo*>(spMsg.get());


	auto sprsp = std::make_shared<rspAudioStreamServiceInfo>();




	spSockContainer->Send(sprsp);
}