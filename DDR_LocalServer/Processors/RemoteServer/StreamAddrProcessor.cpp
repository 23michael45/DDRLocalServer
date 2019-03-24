#include "StreamAddrProcessor.h"
#include "../../Managers/StreamRelayServiceManager.h"
#include "../../Managers/GlobalManager.h"
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
	rspStreamAddr* pRaw = reinterpret_cast<rspStreamAddr*>(spMsg.get());


	auto sprsp = std::make_shared<rspStreamAddr>();

	//if (pRaw->networktype() == ChannelNetworkType::Remote)
	{
		/*auto spSession = GlobalManager::Instance()->GetTcpServer()->GetSessionByType(eLSMStreamRelay);
		if (spSession)
		{
			auto channels = StreamRelayServiceManager::Instance()->GetAVChannelsConfig();
			for (auto channel : channels)
			{

				if (channel.networktype() == ChannelNetworkType::Remote)
				{
					auto *pChannel = sprsp->add_channels();

					pChannel->set_srcaddr(channel.dst());
					pChannel->set_streamtype(channel.streamtype());
					pChannel->set_networktype(channel.networktype());
					pChannel->set_rate(channel.rate());
					pChannel->set_srcname(channel.srcname());

				}
			}


		}*/
		/*else
		{
			sprsp->set_error("LS_Err_NoStreamRelayService_Connected");

		}*/


	}


	spSockContainer->Send(sprsp);
}
