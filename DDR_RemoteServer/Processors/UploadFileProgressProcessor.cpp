
#include <memory>
#include "../../../Shared/proto/BaseCmd.pb.h"
#include "UploadFileProgressProcessor.h"
#include "../../../Shared/src/Utility/DDRMacro.h"
#include "../../../Shared/src/Utility/Logger.h"
#include "../Managers/GlobalManager.h"
#include "../Managers/FileManager.h"

using namespace DDRFramework;
using namespace DDRCommProto;

UploadFileProgressProcessor::UploadFileProgressProcessor(BaseMessageDispatcher& dispatcher) :BaseProcessor(dispatcher)
{
}


UploadFileProgressProcessor::~UploadFileProgressProcessor()
{
}

void UploadFileProgressProcessor::Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{

	notifyUploadFileProgress* pRaw = reinterpret_cast<notifyUploadFileProgress*>(spMsg.get());
	if (pRaw)
	{
		if (MsgRouterManager::Instance()->ReturnPassNode(spHeader, m_Passnode))
		{

			reqRemoteFileAddress* preq = (reqRemoteFileAddress*)(m_Passnode.intptrdata(0));
			if (preq)
			{

				auto sprsp = std::make_shared<rspRemoteFileAddress>();

				for (auto fmt : preq->filenames())
				{
					DebugLog("UploadFileProgressProcessor %s", fmt.c_str());
					auto files = FileManager::Instance()->Match(fmt);

					for (auto fullpath : files)
					{
						auto httpaddr = FileManager::Instance()->GetHttpServerUrl(FileManager::Instance()->GetRelativeDirFromFull(fullpath));
						sprsp->add_fileaddrlist(httpaddr);
					}
				}
			}

			delete preq;

		}
	}


}

void UploadFileProgressProcessor::AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{


}
