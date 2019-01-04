
#include <memory>
#include "../../../Shared/proto/BaseCmd.pb.h"
#include "FileStatusProcessor.h"
#include "../../../Shared/src/Utility/DDRMacro.h"
#include "../../../Shared/src/Utility/Logger.h"
#include "../../../Shared/src/Network/TcpServerBase.h"
#include "../Managers/StreamRelayServiceManager.h"

using namespace DDRFramework;
using namespace DDRCommProto;

FileStatusProcessor::FileStatusProcessor(BaseMessageDispatcher& dispatcher) :BaseProcessor(dispatcher)
{
}


FileStatusProcessor::~FileStatusProcessor()
{
}

void FileStatusProcessor::Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{

	ansFileStatus* pRaw = reinterpret_cast<ansFileStatus*>(spMsg.get());



	auto sprsp = std::make_shared<rspFileAddress>();

	auto spStreamRelaySession = std::dynamic_pointer_cast<TcpSessionBase>(spSockContainer->GetTcp());
	if (spStreamRelaySession)
	{
		sprsp->set_tarservicetype(spStreamRelaySession->GetLoginInfo().type());
		sprsp->set_filetype(pRaw->filetype());
		for (auto file : pRaw->fileaddrlist())
		{
			sprsp->add_fileaddrlist(file);
		}


		//do send to client

		/*std::map<std::shared_ptr<TcpSessionBase>, std::shared_ptr<TcpSessionBase>>& map = StreamRelayServiceManager::Instance()->m_WaitingSessionPare;
		if (map.find(spStreamRelaySession) != map.end() && map[spStreamRelaySession])
		{
			auto spClientSession = map[spStreamRelaySession];
			spClientSession->Send(sprsp);
			map.erase(spStreamRelaySession);
		}*/

	}

}

void FileStatusProcessor::AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{


}
