#include "VirtualServerDispatcher.h"

#include "../../../Shared/proto/SimulationCmd.pb.h"
#include "../Processors/DifferentialRPMProcessor.h"
#include "VirtualTcpServer.h"

using namespace DDRCommProto;
using namespace DDRFramework;

VirtualServerDispatcher::VirtualServerDispatcher()
{


	RegisterProcessor(req, DifferentialRPM)


}


VirtualServerDispatcher::~VirtualServerDispatcher()
{
}

void VirtualServerDispatcher::Dispatch(std::shared_ptr< DDRFramework::BaseSocketContainer> spParentSocketContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{
	BaseMessageDispatcher::Dispatch(spParentSocketContainer, spHeader, spMsg);
}
