

#include <memory>
#include "../../../Shared/proto/BaseCmd.pb.h"
#include "RemoteFileAddressProcessor.h"
#include "../../../Shared/src/Utility/DDRMacro.h"
#include "../../../Shared/src/Utility/Logger.h"
#include "../Managers//GlobalManager.h"
#include "../Managers//FileManager.h"


#include "eventpp/callbacklist.h"
#include "eventpp/eventdispatcher.h"
#include "eventpp/eventqueue.h"
using namespace DDRFramework;
using namespace DDRCommProto;

RemoteFileAddressProcessor::RemoteFileAddressProcessor(BaseMessageDispatcher& dispatcher) :BaseProcessor(dispatcher)
{
}


RemoteFileAddressProcessor::~RemoteFileAddressProcessor()
{
}

void RemoteFileAddressProcessor::Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{


}
void RemoteFileAddressProcessor::AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{
	reqRemoteFileAddress* pRaw = reinterpret_cast<reqRemoteFileAddress*>(spMsg.get());

	if (pRaw)
	{
		//Record Mannually ,Router Info
		auto pNode = MsgRouterManager::Instance()->RecordPassNode(spHeader, spSockContainer->GetTcp());
		reqRemoteFileAddress* pData = new reqRemoteFileAddress();
		pData->CopyFrom(*pRaw);
		pNode->add_intptrdata((size_t)pData);


		auto spnotify = std::make_shared<notifyUploadFile>();
		spnotify->set_tarservicetype(pRaw->tarservicetype());
		spnotify->set_httpaddr(FileManager::Instance()->GetHttpServerUrl(""));


		for (auto fmt : pRaw->filenames())
		{
			spnotify->add_filefmt(fmt);
		}



		for (auto fmt : pRaw->filenames())
		{
			DebugLog("RemoteFileAddressProcessor %s", fmt.c_str());
			auto files = FileManager::Instance()->Match(fmt);

			for (auto fullpath : files)
			{
				spnotify->add_existfiles(FileManager::Instance()->GetRelativeDirFromFull(fullpath));
			}
		}



		eCltType toType = pRaw->tarservicetype();
		if ((toType & eAllLSM) != 0)
		{
			auto spFromSession = dynamic_pointer_cast<RemoteServerTcpSession>(spSockContainer->GetTcp());
			if (spFromSession)
			{
				auto spToSession = GlobalManager::Instance()->GetTcpServer()->GetLSSession(spFromSession->GetBindLSUDID());
				if (spToSession)
				{
					spToSession->Send(spHeader, spnotify);
				}
				else
				{
					DebugLog("RemoteFileAddressProcessor No Session Conncected type:%i", pRaw->tarservicetype());
				}
			}

		}
	}


}
void RemoteFileAddressProcessor::Lengancy(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{
	reqRemoteFileAddress* pRaw = reinterpret_cast<reqRemoteFileAddress*>(spMsg.get());

	if (pRaw)
	{
		//Record Mannually ,Router Info
		MsgRouterManager::Instance()->RecordPassNode(spHeader, spSockContainer->GetTcp());


		std::vector<std::string> matchedfiles;
		for (auto fmt : pRaw->filenames())
		{
			DebugLog("RemoteFileAddressProcessor %s", fmt.c_str());
			auto files = FileManager::Instance()->Match(fmt);

			for (auto fullpath : files)
			{
				//std::string httpaddr = FileManager::Instance()->GetHttpServerUrl(file);
				matchedfiles.push_back(fullpath);
			}
		}




		bool allexsit = true;
		auto spnotify = std::make_shared<notifyUploadFile>();
		spnotify->set_tarservicetype(pRaw->tarservicetype());
		spnotify->set_httpaddr(FileManager::Instance()->GetHttpServerUrl(""));

		auto sprsp = std::make_shared<rspRemoteFileAddress>();

		for (auto fullpath : matchedfiles)
		{
			if (FileManager::Instance()->FileExist(fullpath))
			{
				spnotify->add_existfiles(FileManager::Instance()->GetRelativeDirFromFull(fullpath));
				allexsit = false;
			}
			else
			{
				sprsp->add_fileaddrlist(FileManager::Instance()->GetHttpServerUrl(FileManager::Instance()->GetRelativeDirFromFull(fullpath)));
			}
		}



		if (allexsit)
		{
			spSockContainer->GetTcp()->Send(sprsp);
		}
		else
		{
			auto spFromSession = dynamic_pointer_cast<RemoteServerTcpSession>(spSockContainer->GetTcp());
			if (spFromSession)
			{
				auto spToSession = GlobalManager::Instance()->GetTcpServer()->GetLSSession(spFromSession->GetBindLSUDID());
				if (spToSession)
				{
					spToSession->Send(spHeader, spnotify);
				}
				else
				{
					DebugLog("RemoteFileAddressProcessor No Bind LS Session");
				}
			}
		}

	}

}
