#include "LocalServerDispatcher.h"

#include "../../Shared/proto/BaseCmd.pb.h"
#include "LoginProcessor.h"
#include "HeartBeatProcessor.h"

using namespace DDRCommProto;
using namespace DDRFramework;

LocalServerDispatcher::LocalServerDispatcher()
{
	heartBeat heartBeat;
	m_ProcessorMap[heartBeat.GetTypeName()] = std::make_shared<HeartBeatProcessor>();

	reqLogin rspSignin;
	m_ProcessorMap[rspSignin.GetTypeName()] = std::make_shared<LoginProcessor>();

}


LocalServerDispatcher::~LocalServerDispatcher()
{
}
