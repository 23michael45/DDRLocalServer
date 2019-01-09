

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

	reqRemoteServerList* pRaw = reinterpret_cast<reqRemoteServerList*>(spMsg.get());



	auto sprsp = std::make_shared<rspRemoteServerList>();

	for (auto server : GlobalManager::Instance()->GetServerVec())
	{

		auto* p = sprsp->add_servers();
		p->CopyFrom(server);
	}
	auto ip = sprsp->servers(0).ip();

	spSockContainer->Send(sprsp);
}

void RemoteServerListProcessor::AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{


}
