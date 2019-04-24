#include "VirtualServerDispatcher.h"

#include "proto/SimulationCmd.pb.h"
#include "../Processors/DifferentialRPMProcessor.h"
#include "../Processors/StartFromPoseProcessor.h"
#include "../Processors/LaunchAMCProcessor.h"
#include "VirtualTcpServer.h"

using namespace DDRCommProto;
using namespace DDRFramework;

VirtualServerDispatcher::VirtualServerDispatcher()
{


	RegisterProcessor(req, DifferentialRPM)
		RegisterProcessor(req, LaunchAMC)
		RegisterProcessor(req, StartFromPose)


}


VirtualServerDispatcher::~VirtualServerDispatcher()
{
}

void VirtualServerDispatcher::Dispatch(std::shared_ptr< DDRFramework::BaseSocketContainer> spParentSocketContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{
	BaseMessageDispatcher::Dispatch(spParentSocketContainer, spHeader, spMsg);
}
