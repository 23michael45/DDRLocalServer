#include "LocalServerDispatcher.h"

#include "../../../Shared/proto/BaseCmd.pb.h"
#include "../Processors/LoginProcessor.h"
#include "../Processors/StreamRelay/VideoStreamInfoProcessor.h"
#include "../Processors/StreamRelay/AudioStreamInfoProcessor.h"
#include "../Processors/HeartBeatProcessor.h"

using namespace DDRCommProto;
using namespace DDRFramework;

LocalServerDispatcher::LocalServerDispatcher()
{
	HeartBeat hBeat;
	m_ProcessorMap[hBeat.GetTypeName()] = std::make_shared<HeartBeatProcessor>(*this);


	RegisterProcessor(req, Login)
	RegisterProcessor(req, VideoStreamInfo)
	RegisterProcessor(req, AudioStreamInfo)

}


LocalServerDispatcher::~LocalServerDispatcher()
{
}
