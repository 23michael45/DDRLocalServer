
#include <memory>
#include "proto/BaseCmd.pb.h"
#include "FileAddressProcessor.h"
#include "src/Utility/DDRMacro.h"
#include "src/Utility/Logger.h"
#include "src/Utility/CommonFunc.h"
#include "thirdparty/cppfs/include/cppfs/windows/LocalFileSystem.h"
#include "thirdparty/cppfs/include/cppfs/FilePath.h"
#include "GlobalManager.h"

#include <iostream>
#include <fstream>
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


	cppfs::FilePath path(DDRFramework::getexepath());
	auto root = path.directoryPath();
	std::vector<std::string> outfiles;
	std::vector<std::string> urls;
	for (auto url : pRaw->fileaddrlist())
	{
		//do curl get;

		urls.push_back(url);

		cppfs::FilePath fpath(url);
		auto filename = fpath.fileName();
		outfiles.push_back(root + filename);



	}





	spHttpSession = std::make_shared<HttpSession>();
	spHttpSession->BindOnGetDoneFunc(std::bind(&FileAddressProcessor::OnGetDone,this, std::placeholders::_1));
	spHttpSession->DoGet(urls, outfiles);


}

void FileAddressProcessor::AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{


}

void FileAddressProcessor::OnGetDone(float f)
{
	DebugLog("FileAddressProcessor OnGetDone %f",f);
}
