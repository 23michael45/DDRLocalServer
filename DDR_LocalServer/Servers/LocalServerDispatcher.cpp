#include "LocalServerDispatcher.h"

#include "../../../Shared/proto/BaseCmd.pb.h"
#include "../Processors/LoginProcessor.h"
#include "../Processors/StreamRelay/StreamServiceInfoProcessor.h"
#include "../Processors/HeartBeatProcessor.h"
#include "../Processors/Client/StreamAddrProcessor.h"
#include "../Processors/Client/FileAddressProcessor.h"
#include "../Processors/StreamRelay/StreamRelayAlarmProcessor.h"
#include "../Processors/FileStatusProcessor.h"

using namespace DDRCommProto;
using namespace DDRFramework;

LocalServerDispatcher::LocalServerDispatcher()
{
	HeartBeat hBeat;
	m_ProcessorMap[hBeat.GetTypeName()] = std::make_shared<HeartBeatProcessor>(*this);

		//Global
		RegisterProcessor(req, Login)


		//Client
		RegisterProcessor(req, StreamAddr)
		RegisterProcessor(req, FileAddress)

		//Stream Relay
		RegisterProcessor(req, StreamServiceInfo)
		RegisterProcessor(req, StreamRelayAlarm)
		RegisterProcessor(ans, FileStatus)


}


LocalServerDispatcher::~LocalServerDispatcher()
{
}
