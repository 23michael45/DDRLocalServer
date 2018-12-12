#include "LocalServerDispatcher.h"

#include "../../Shared/proto/BaseCmd.pb.h"
#include "LoginProcessor.h"
#include "HeartBeatProcessor.h"

using namespace DDRCommProto;
using namespace DDRFramework;

LocalServerDispatcher::LocalServerDispatcher()
{
	heartBeat hBeat;
	m_ProcessorMap[hBeat.GetTypeName()] = std::make_shared<HeartBeatProcessor>(*this);

	reqLogin rspSignin;
	m_ProcessorMap[rspSignin.GetTypeName()] = std::make_shared<LoginProcessor>(*this);

}


LocalServerDispatcher::~LocalServerDispatcher()
{
}
