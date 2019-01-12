
#include <memory>
#include "../../../Shared/proto/BaseCmd.pb.h"
#include "FileStatusProcessor.h"
#include "../../../Shared/src/Utility/DDRMacro.h"
#include "../../../Shared/src/Utility/Logger.h"
#include "../Managers/GlobalManager.h"
#include "../Managers/FileManager.h"
#include "../../../Shared/thirdparty/cppfs/include/cppfs/windows/LocalFileSystem.h"
#include "../../../Shared/thirdparty/cppfs/include/cppfs/FilePath.h"
#include "../../../Shared/src/Network/HttpClient.h"

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

	if (pRaw)
	{

		auto spnotify = std::make_shared<notifyUploadFile>();

		spnotify->set_httpaddr(FileManager::Instance()->GetHttpServerUrl());

		for (auto file : pRaw->fileaddrlist())
		{
			if (!FileManager::Instance()->FileExist(FileManager::Instance()->GetFullDirFromRelative(file)))
			{
				spnotify->add_filenames(file);
			}
		}

		spSockContainer->GetTcp()->Send(spnotify);
	}


}

void FileStatusProcessor::AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{


}
