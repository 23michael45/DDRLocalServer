#include "LSClientDispatcher.h"
#include "../../../Shared/proto/BaseCmd.pb.h"

#include "../Processors/RemoteServer/RemoteServerListProcessor.h"
#include "../Processors/RemoteServer/UploadFileProcessor.h"
using namespace DDRCommProto;
using namespace DDRFramework;

LSClientDispatcher::LSClientDispatcher()
{		

	RegisterProcessor(notify, UploadFile)
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
