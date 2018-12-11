#include "HeartBeatProcessor.h"
#include "../../Shared/proto/BaseCmd.pb.h"
#include "../../Shared/src/Utility/DDRMacro.h"
using namespace DDRFramework;
using namespace DDRCommProto;


HeartBeatProcessor::HeartBeatProcessor()
{
}


HeartBeatProcessor::~HeartBeatProcessor()
{
}


void HeartBeatProcessor::Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{
	auto bodytype = spHeader->bodytype();

	heartBeat* pRaw = reinterpret_cast<heartBeat*>(spMsg.get());

	std::string whatever = pRaw->whatever();


	auto sprsp = std::make_shared<heartBeat>();
	sprsp->set_whatever("heartBeat whatever");

	spSockContainer->Send(sprsp);

}
