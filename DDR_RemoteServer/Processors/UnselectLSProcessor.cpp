
#include <memory>
#include "../../../Shared/proto/BaseCmd.pb.h"
#include "UnselectLSProcessor.h"
#include "../../../Shared/src/Utility/DDRMacro.h"
#include "../../../Shared/src/Utility/Logger.h"
#include "../Managers/GlobalManager.h"

using namespace DDRFramework;
using namespace DDRCommProto;

UnselectLSProcessor::UnselectLSProcessor(BaseMessageDispatcher& dispatcher) :BaseProcessor(dispatcher)
{
}


UnselectLSProcessor::~UnselectLSProcessor()
{
}

void UnselectLSProcessor::Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{

	reqUnselectLS* pRaw = reinterpret_cast<reqUnselectLS*>(spMsg.get());

	if (pRaw)
	{
		auto& map = GlobalManager::Instance()->GetTcpServer()->GetClientMap();

		auto spClientSession = dynamic_pointer_cast<RemoteServerTcpSession>(spSockContainer->GetTcp());
		std::string username = spClientSession->GetRemoteLoginInfo().username();
		if (map.find(username) != map.end())
		{
			map[username]->ReleaseLS(pRaw->udid());
		}
		else
		{
			DebugLog("RemoteClient Not Login");
		}

	}

}