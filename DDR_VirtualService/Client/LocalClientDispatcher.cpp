#include "LocalClientDispatcher.h"
#include "../../../Shared/proto/BaseCmd.pb.h"

#include "../Processors/LoginProcessor.h"
#include "../Processors/FileAddressProcessor.h"

using namespace DDRCommProto;
using namespace DDRFramework;

LocalClientDispatcher::LocalClientDispatcher()
{

	RegisterProcessor(rsp, Login)
	RegisterProcessor(req, FileAddress)

}


LocalClientDispatcher::~LocalClientDispatcher()
{
}
