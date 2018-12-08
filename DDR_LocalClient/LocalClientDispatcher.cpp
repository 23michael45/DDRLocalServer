#include "LocalClientDispatcher.h"
#include "../../Shared/proto/BaseCmd.pb.h"

#include "LoginProcessor.h"

using namespace DDRCommProto;
using namespace DDRFramework;

LocalClientDispatcher::LocalClientDispatcher(std::shared_ptr<TcpSocketContainer> sp) : BaseMessageDispatcher::BaseMessageDispatcher(sp)
{

	respLogin respLogin;
	m_ProcessorMap[respLogin.GetTypeName()] =  std::make_shared<LoginProcessor>();
}


LocalClientDispatcher::~LocalClientDispatcher()
{
}
