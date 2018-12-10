#include "LocalServerDispatcher.h"

#include "../../Shared/proto/BaseCmd.pb.h"
#include "LoginProcessor.h"

using namespace DDRCommProto;
using namespace DDRFramework;

LocalServerDispatcher::LocalServerDispatcher()
{
	reqLogin rspSignin;
	m_ProcessorMap[rspSignin.GetTypeName()] = std::make_shared<LoginProcessor>();

}


LocalServerDispatcher::~LocalServerDispatcher()
{
}
