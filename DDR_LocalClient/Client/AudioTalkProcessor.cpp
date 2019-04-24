
#include <memory>
#include "proto/BaseCmd.pb.h"
#include "AudioTalkProcessor.h"
#include "src/Utility/DDRMacro.h"
#include "src/Utility/Logger.h"
#include "GlobalManager.h"

using namespace DDRFramework;
using namespace DDRCommProto;

AudioTalkProcessor::AudioTalkProcessor(BaseMessageDispatcher& dispatcher) :BaseProcessor(dispatcher)
{
}


AudioTalkProcessor::~AudioTalkProcessor()
{
}

void AudioTalkProcessor::Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{

	rspAudioTalk* pRaw = reinterpret_cast<rspAudioTalk*>(spMsg.get());

	DebugLog("AudioTalkProcessor %d" , pRaw->status());
}

void AudioTalkProcessor::AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{


}
