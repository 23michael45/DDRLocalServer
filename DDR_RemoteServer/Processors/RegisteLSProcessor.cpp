

#include <memory>
#include "../../../Shared/proto/BaseCmd.pb.h"
#include "RegisteLSProcessor.h"
#include "../../../Shared/src/Utility/DDRMacro.h"
#include "../../../Shared/src/Utility/Logger.h"
#include "../Managers/GlobalManager.h"

using namespace DDRFramework;
using namespace DDRCommProto;

RegisteLSProcessor::RegisteLSProcessor(BaseMessageDispatcher& dispatcher) :BaseProcessor(dispatcher)
{
}


RegisteLSProcessor::~RegisteLSProcessor()
{
}

void RegisteLSProcessor::Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{

	reqRegisteLS* pRaw = reinterpret_cast<reqRegisteLS*>(spMsg.get());

	if (pRaw)
	{

		auto spTcp = spSockContainer->GetTcp();
		if (spTcp)
		{
			auto spServerSessionTcp = dynamic_pointer_cast<RemoteServerTcpSession>(spTcp);
			if (spServerSessionTcp)
			{
				spServerSessionTcp->AssignRegisteLSInfo(*pRaw);

			}
		}

	}






}

void RegisteLSProcessor::AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{


}
