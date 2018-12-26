
#include <memory>
#include "../../../Shared/proto/BaseCmd.pb.h"
#include "FileAddressProcessor.h"
#include "../../../Shared/src/Utility/DDRMacro.h"
#include "../../../Shared/src/Utility/Logger.h"
#include "../../../Shared/thirdparty/cppfs/include/cppfs/windows/LocalFileSystem.h"
#include "../../../Shared/thirdparty/cppfs/include/cppfs/FilePath.h"
#include "GlobalManager.h"
#include "../../../Shared/thirdparty/curl-asio/curl-asio.hpp"

#include <iostream>
#include <fstream>
#include "../../../Shared/src/Network/HttpClient.h"
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

	rspFileAddress* pRaw = reinterpret_cast<rspFileAddress*>(spMsg.get());

	for (auto url : pRaw->fileaddrlist())
	{
		//do curl get;

		cppfs::FilePath path(cppfs::getexepath());
		auto root = path.directoryPath();


		cppfs::FilePath fpath(url);
		auto filename = fpath.fileName();

		HttpSession httpSession;
		httpSession.DoGet(url, root + filename);
	}


}

void FileAddressProcessor::AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{


}
