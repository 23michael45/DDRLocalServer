

#include <memory>
#include "proto/RemoteCmd.pb.h"
#include "RtspStreamUploadAddrProcessor.h"
#include "src/Utility/DDRMacro.h"
#include "src/Utility/Logger.h"
#include "../Managers/StreamProxyManager.h"
#include "../Servers/RemoteTcpServer.h"

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
	if (pRaw)
	{

		auto sprsp = std::make_shared<rspRtspStreamUploadAddr>();

		std::shared_ptr<RemoteServerTcpSession> spRSTcpSession = std::dynamic_pointer_cast<RemoteServerTcpSession>(spSockContainer->GetTcp());
		if (spRSTcpSession)
		{
			std::string udid = spRSTcpSession->GetRegisteLSInfo().udid();



			for (auto channel : pRaw->channels())
			{
				RemoteStreamChannel rchannel = StreamProxyManager::Instance()->AllocMostIdleProxyUpload(udid, channel);



				auto pchannel = sprsp->add_channels();
				pchannel->CopyFrom(rchannel);
			}
		}
	
	
		if (sprsp->channels_size() <= 0)
		{
			sprsp->set_error("No Rtsp Proxy Alloc");
		}

		spSockContainer->Send(sprsp);
	}

}

void RtspStreamUploadAddrProcessor::AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{


}
