#include "StreamAddrProcessor.h"
#include "../../Managers/StreamRelayServiceManager.h"
#include "../../../Shared/proto/BaseCmd.pb.h"
#include "../../../Shared/src/Utility/DDRMacro.h"
using namespace DDRFramework;
using namespace DDRCommProto;


StreamAddrProcessor::StreamAddrProcessor(BaseMessageDispatcher& dispatcher) : BaseProcessor::BaseProcessor(dispatcher)
{
}


StreamAddrProcessor::~StreamAddrProcessor()
{
}


void StreamAddrProcessor::Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{
	auto bodytype = spHeader->bodytype();
	reqStreamAddr* pRaw = reinterpret_cast<reqStreamAddr*>(spMsg.get());


	auto sprsp = std::make_shared<rspStreamAddr>();
	if (pRaw->networktype() == ChannelNetworkType::Local)
	{
		auto spSession = StreamRelayServiceManager::Instance()->GetServerSession();
		if (spSession)
		{
			auto *pChannel = sprsp->add_channels();
			std::string dstip = spSession->GetSocket().remote_endpoint().address().to_string();
			pChannel->set_dst(dstip);
			pChannel->add_dstport(StreamRelayServiceManager::Instance()->GetServerTcpPort());
			pChannel->set_networktype(ChannelNetworkType::Local);

		}

	}
	else if (pRaw->networktype() == ChannelNetworkType::Remote)
	{



	}


	spSockContainer->Send(sprsp);
}
