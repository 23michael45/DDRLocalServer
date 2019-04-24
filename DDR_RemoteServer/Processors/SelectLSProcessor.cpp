
#include <memory>
#include "proto/BaseCmd.pb.h"
#include "SelectLSProcessor.h"
#include "src/Utility/DDRMacro.h"
#include "src/Utility/Logger.h"
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


			auto sprsp = std::make_shared<rspSelectLS>();
			sprsp->set_udid(pRaw->udid());
			sprsp->set_error("");
			spSockContainer->Send(sprsp);
		}
		else
		{
			auto sprsp = std::make_shared<rspSelectLS>();
			sprsp->set_error("RemoteClient Not Login");
			spSockContainer->Send(sprsp);
		}

	}


}

void SelectLSProcessor::AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{


}
