#include "LocalServerUdpDispatcher.h"
#include "LocalServerUdpProcessor.h"
#include "../../Shared/proto/BaseCmd.pb.h"
#include "LoginProcessor.h"
#include "HeartBeatProcessor.h"


using namespace DDRCommProto;
LocalServerUdpDispatcher::LocalServerUdpDispatcher()
{

	broadcastServerInformation bcServerInfo;
	m_ProcessorMap[bcServerInfo.GetTypeName()] = std::make_shared<LocalServerUdpProcessor>(*this);
}


LocalServerUdpDispatcher::~LocalServerUdpDispatcher()
{
}
