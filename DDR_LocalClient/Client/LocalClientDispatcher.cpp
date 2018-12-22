#include "LocalClientDispatcher.h"
#include "../../../Shared/proto/BaseCmd.pb.h"

#include "LoginProcessor.h"
#include "StreamAddrProcessor.h"
#include "AlarmProcessor.h"
using namespace DDRCommProto;
using namespace DDRFramework;

LocalClientDispatcher::LocalClientDispatcher()
{

		RegisterProcessor(rsp, Login)
		RegisterProcessor(rsp, StreamAddr)
		RegisterProcessor(notify, Alarm)
}


LocalClientDispatcher::~LocalClientDispatcher()
{
}
