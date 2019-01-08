#include "BroadcastServerDispatcher.h"

#include "../../../Shared/proto/BaseCmd.pb.h"
#include "../Processors/RemoteServerListProcessor.h"

using namespace DDRCommProto;
using namespace DDRFramework;

BroadcastServerDispatcher::BroadcastServerDispatcher()
{

	//Global
	RegisterProcessor(req, RemoteServerList)



}


BroadcastServerDispatcher::~BroadcastServerDispatcher()
{
}
