#include "LSClientDispatcher.h"
#include "../../../Shared/proto/BaseCmd.pb.h"

#include "RemoteServerListProcessor.h"

using namespace DDRCommProto;
using namespace DDRFramework;

LSClientDispatcher::LSClientDispatcher()
{		
	
}


LSClientDispatcher::~LSClientDispatcher()
{
}

//----------------------------------------------------------------------------------------------------------------------------------------

LSBroadcastReceiveClientDispatcher::LSBroadcastReceiveClientDispatcher()
{
	RegisterProcessor(rsp, RemoteServerList)

}


LSBroadcastReceiveClientDispatcher::~LSBroadcastReceiveClientDispatcher()
{
}
