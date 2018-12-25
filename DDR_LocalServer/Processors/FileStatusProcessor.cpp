
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

	auto spSession = std::dynamic_pointer_cast<TcpSessionBase>(spSockContainer->GetTcp());
	if (spSession)
	{
		sprsp->set_tarservicetype(spSession->GetLoginInfo().type());
		sprsp->set_filetype(pRaw->filetype());
		for (auto file : pRaw->fileaddrlist())
		{
			sprsp->add_fileaddrlist(file);
		}

		auto map = StreamRelayServiceManager::Instance()->m_WaitingSessionPare;
		if (map.find(spSession) != map.end() && map[spSession])
		{
			map[spSession]->Send(sprsp);
			map.erase(spSession);
		}

	}

}

void FileStatusProcessor::AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{


}
