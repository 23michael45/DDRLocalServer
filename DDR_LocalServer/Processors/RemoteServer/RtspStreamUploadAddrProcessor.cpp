

#include <memory>
#include "../../../Shared/proto/RemoteCmd.pb.h"
#include "RtspStreamUploadAddrProcessor.h"
#include "../../../Shared/src/Utility/DDRMacro.h"
#include "../../../Shared/src/Utility/Logger.h"

using namespace DDRFramework;
using namespace DDRCommProto;

RtspStreamUploadAddrProcessor::RtspStreamUploadAddrProcessor(BaseMessageDispatcher& dispatcher) :BaseProcessor(dispatcher)
{
}


RtspStreamUploadAddrProcessor::~RtspStreamUploadAddrProcessor()
{
}

void RtspStreamUploadAddrProcessor::Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{

	reqRtspStreamUploadAddr* pRaw = reinterpret_cast<reqRtspStreamUploadAddr*>(spMsg.get());


}

void RtspStreamUploadAddrProcessor::AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{


}
