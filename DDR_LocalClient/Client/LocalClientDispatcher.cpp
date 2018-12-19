#include "LocalClientDispatcher.h"
#include "../../../Shared/proto/BaseCmd.pb.h"

#include "LoginProcessor.h"

using namespace DDRCommProto;
using namespace DDRFramework;

LocalClientDispatcher::LocalClientDispatcher()
{

	RegisterProcessor(rsp, Login)
}


LocalClientDispatcher::~LocalClientDispatcher()
{
}
