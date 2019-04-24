

#include <memory>
#include "proto/BaseCmd.pb.h"
#include "RemoteFileAddressProcessor.h"
#include "src/Utility/DDRMacro.h"
#include "src/Utility/Logger.h"
#include "src/Utility/CommonFunc.h"


#include "thirdparty/eventpp/callbacklist.h"
#include "thirdparty/eventpp/eventdispatcher.h"
#include "thirdparty/eventpp/eventqueue.h"


#include "thirdparty/cppfs/include/cppfs/windows/LocalFileSystem.h"
#include "thirdparty/cppfs/include/cppfs/FilePath.h"
#include "GlobalManager.h"

#include <iostream>
#include <fstream>
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
	rspFileAddress* pRaw = reinterpret_cast<rspFileAddress*>(spMsg.get());




	cppfs::FilePath path(DDRFramework::getexepath());
	auto root = path.directoryPath();
	std::vector<std::string> outfiles;
	std::vector<std::string> urls;
	for (auto url : pRaw->fileaddrlist())
	{
		//do curl get;urls.push_back(url);

		cppfs::FilePath fpath(url);
		auto filename = fpath.fileName();
		outfiles.push_back(root + filename);

		urls.push_back(url);

	}


	spHttpSession = std::make_shared<HttpSession>();
	spHttpSession->BindOnGetDoneFunc(std::bind(&RemoteFileAddressProcessor::OnGetDone, this, std::placeholders::_1));
	spHttpSession->DoGet(urls, outfiles);
}

void RemoteFileAddressProcessor::OnGetDone(float f)
{
	DebugLog("FileAddressProcessor OnGetDone %f", f);
}
