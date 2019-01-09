

#include <memory>
#include "../../../Shared/proto/BaseCmd.pb.h"
#include "RemoteServerListProcessor.h"
#include "../../../Shared/src/Utility/DDRMacro.h"
#include "../../../Shared/src/Utility/Logger.h"
#include "../Managers/GlobalManager.h"

using namespace DDRFramework;
using namespace DDRCommProto;

RemoteServerListProcessor::RemoteServerListProcessor(BaseMessageDispatcher& dispatcher) :BaseProcessor(dispatcher)
{
}


RemoteServerListProcessor::~RemoteServerListProcessor()
{
}

void RemoteServerListProcessor::Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{
	rspRemoteServerList* pRaw = reinterpret_cast<rspRemoteServerList*>(spMsg.get());

	for (auto server : pRaw->servers())
	{
		DebugLog("Get Server:%s", server.ip().c_str());
	}
}

void RemoteServerListProcessor::AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{


}
