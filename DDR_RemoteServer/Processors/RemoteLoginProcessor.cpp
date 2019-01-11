

#include <memory>
#include "../../../Shared/proto/BaseCmd.pb.h"
#include "RemoteLoginProcessor.h"
#include "../../../Shared/src/Utility/DDRMacro.h"
#include "../../../Shared/src/Utility/Logger.h"
#include "../Managers/GlobalManager.h"

using namespace DDRFramework;
using namespace DDRCommProto;

RemoteLoginProcessor::RemoteLoginProcessor(BaseMessageDispatcher& dispatcher) :BaseProcessor(dispatcher)
{
}


RemoteLoginProcessor::~RemoteLoginProcessor()
{
}

void RemoteLoginProcessor::Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{

	reqRemoteLogin* pRaw = reinterpret_cast<reqRemoteLogin*>(spMsg.get());
	if (pRaw)
	{

		auto sprsp = std::make_shared<rspRemoteLogin>();

		std::string name = pRaw->username();
		std::string pwd = pRaw->userpwd();
		eCltType type = pRaw->type();
		bool b = DBManager::Instance()->VerifyUser(name, pwd);

		if (b)
		{
			auto spClientSession = dynamic_pointer_cast<RemoteServerTcpSession>(spSockContainer->GetTcp());
			spClientSession->AssignRemoteLoginInfo(*pRaw);




			auto& lsmap = GlobalManager::Instance()->GetTcpServer()->GetLSMap();

			for (auto pair : lsmap)
			{
				auto spLSSession =pair.second;

				auto pls = sprsp->add_lslist();
				pls->set_name(spLSSession->GetRegisteLSInfo().name());
				pls->set_udid(spLSSession->GetRegisteLSInfo().udid());
				
			}


		}
		else
		{
			sprsp->set_retcode(rspRemoteLogin_eRemoteLoginRetCode_incorrect_password);

		}


		spSockContainer->Send(sprsp);
	}


}

void RemoteLoginProcessor::AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{


}