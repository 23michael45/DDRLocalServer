#include "LocalClientUdpDispatcher.h"
#include "LocalClientUdpProcessor.h"
#include "../../Shared/proto/BaseCmd.pb.h"
#include "LoginProcessor.h"


using namespace DDRCommProto;
LocalClientUdpDispatcher::LocalClientUdpDispatcher()
{

	broadcastServerInformation bcClientInfo;
	m_ProcessorMap[bcClientInfo.GetTypeName()] = std::make_shared<LocalClientUdpProcessor>(*this);
}


LocalClientUdpDispatcher::~LocalClientUdpDispatcher()
{
}
