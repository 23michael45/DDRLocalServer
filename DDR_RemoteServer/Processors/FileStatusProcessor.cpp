
#include <memory>
#include "proto/BaseCmd.pb.h"
#include "FileStatusProcessor.h"
#include "src/Utility/DDRMacro.h"
#include "src/Utility/Logger.h"
#include "../Managers/GlobalManager.h"
#include "../Managers/FileManager.h"
#include "thirdparty/cppfs/include/cppfs/windows/LocalFileSystem.h"
#include "thirdparty/cppfs/include/cppfs/FilePath.h"
#include "src/Network/HttpClient.h"

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

	ansFileStatus* pRawAns = reinterpret_cast<ansFileStatus*>(spMsg.get());

	if (pRawAns)
	{

	
	}


}

void FileStatusProcessor::AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{


}
