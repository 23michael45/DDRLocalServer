#include "BroadcastServerDispatcher.h"

#include "proto/BaseCmd.pb.h"
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
