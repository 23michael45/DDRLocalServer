

#include <memory>
#include "../../../Shared/proto/BaseCmd.pb.h"
#include "FileAddressProcessor.h"
#include "../../../Shared/src/Utility/DDRMacro.h"
#include "../../../Shared/src/Utility/Logger.h"
#include "../../Managers//GlobalManager.h"
#include "../../Managers//StreamRelayServiceManager.h"


#include "eventpp/callbacklist.h"
#include "eventpp/eventdispatcher.h"
#include "eventpp/eventqueue.h"
#include "../../Servers/LocalTcpServer.h"
using namespace DDRFramework;
using namespace DDRCommProto;

FileAddressProcessor::FileAddressProcessor(BaseMessageDispatcher& dispatcher) :BaseProcessor(dispatcher)
{
}


FileAddressProcessor::~FileAddressProcessor()
{
}

void FileAddressProcessor::Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{


}

void FileAddressProcessor::AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{
	reqFileAddress* pRaw = reinterpret_cast<reqFileAddress*>(spMsg.get());
	SendChk(pRaw);
}

void FileAddressProcessor::SendChk(reqFileAddress* pRaw)
{
	auto sprsp = std::make_shared<chkFileStatus>();
	sprsp->set_filetype(pRaw->filetype());
	for (auto fmt : pRaw->filenames())
	{
		sprsp->add_filenames(fmt);
	}

	if (pRaw->tarservicetype() == eLSMStreamRelay)
	{
		auto spStreamRelaySession = StreamRelayServiceManager::Instance()->GetServerSession();
		if (spStreamRelaySession)
		{
			spStreamRelaySession->Send(sprsp);
		}
		else
		{
			DebugLog("No StreamRelayServer Conncected");
		}

	}
	if (pRaw->tarservicetype() == eLSMFaceRecognition)
	{

	}
	if (pRaw->tarservicetype() == eLSMSlamNavigation)
	{

	}
	if (pRaw->tarservicetype() == eLSMThermalImaging)
	{

	}
	else
	{

	}
}
