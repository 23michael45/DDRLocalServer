
#include <memory>
#include "../../../Shared/proto/BaseCmd.pb.h"
#include "SelectLSProcessor.h"
#include "../../../Shared/src/Utility/DDRMacro.h"
#include "../../../Shared/src/Utility/Logger.h"
#include "../Managers/GlobalManager.h"

using namespace DDRFramework;
using namespace DDRCommProto;

SelectLSProcessor::SelectLSProcessor(BaseMessageDispatcher& dispatcher) :BaseProcessor(dispatcher)
{
}


SelectLSProcessor::~SelectLSProcessor()
{
}

void SelectLSProcessor::Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{

	reqSelectLS* pRaw = reinterpret_cast<reqSelectLS*>(spMsg.get());

	if (pRaw)
	{
		auto& map = GlobalManager::Instance()->GetTcpServer()->GetClientMap();

		auto spClientSession = dynamic_pointer_cast<RemoteServerTcpSession>(spSockContainer->GetTcp());
		std::string username = spClientSession->GetRemoteLoginInfo().username();
		if (map.find(username) != map.end())
		{
			map[username]->BindLS(pRaw->udid());
		}
		else
		{
			DebugLog("RemoteClient Not Login");
		}

	}


}

void SelectLSProcessor::AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{


}
