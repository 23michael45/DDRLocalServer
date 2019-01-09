#include "RemoteServerDispatcher.h"

#include "../../../Shared/proto/BaseCmd.pb.h"
#include "../Processors/RemoteServerListProcessor.h"

using namespace DDRCommProto;
using namespace DDRFramework;

RemoteServerDispatcher::RemoteServerDispatcher()
{

	//Global
	RegisterProcessor(req, RemoteServerList)



}


RemoteServerDispatcher::~RemoteServerDispatcher()
{
}
