#include "LocalClientUdpDispatcher.h"
#include "LocalClientUdpProcessor.h"
#include "proto/BaseCmd.pb.h"
#include "LoginProcessor.h"


using namespace DDRCommProto;
LocalClientUdpDispatcher::LocalClientUdpDispatcher()
{

	bcLSAddr bcClientInfo;
	m_ProcessorMap[bcClientInfo.GetTypeName()] = std::make_shared<LocalClientUdpProcessor>(*this);
}


LocalClientUdpDispatcher::~LocalClientUdpDispatcher()
{
	DebugLog("LocalClientUdpDispatcher Destroy");
}
