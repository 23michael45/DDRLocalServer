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
	reqStreamAddr* pRaw = reinterpret_cast<reqStreamAddr*>(spMsg.get());


	auto sprsp = std::make_shared<rspStreamAddr>();





	if (pRaw->networktype() == ChannelNetworkType::Local)
	{
		auto spSession = GlobalManager::Instance()->GetTcpServer()->GetSessionByType(eLSMStreamRelay);
		if (spSession)
		{

			auto channels = StreamRelayServiceManager::Instance()->GetAVChannelsConfig();
			for (auto channel : channels)
			{
				if (channel.streamtype() == ChannelStreamType::Video)
				{
					auto *pChannel = sprsp->add_channels();

					pChannel->set_srcaddr(channel.src());
					pChannel->set_streamtype(ChannelStreamType::Video);
					pChannel->set_networktype(channel.networktype());
					pChannel->set_rate(channel.rate());

				}
				else if (channel.streamtype() == ChannelStreamType::VideoAudio)
				{
					auto *pChannel = sprsp->add_channels();

					pChannel->set_srcaddr(channel.src());
					pChannel->set_streamtype(ChannelStreamType::VideoAudio);
					pChannel->set_networktype(channel.networktype());
					pChannel->set_rate(channel.rate());

				}
				else if (channel.streamtype() == ChannelStreamType::Audio)
				{
					auto *pChannel = sprsp->add_channels();



					std::string dstip = spSession->GetSocket().remote_endpoint().address().to_string();
					pChannel->set_srcaddr(dstip);
					pChannel->add_srcport(StreamRelayServiceManager::Instance()->GetServerTcpPort());
					pChannel->set_streamtype(ChannelStreamType::Audio);
					pChannel->set_networktype(channel.networktype());
					pChannel->set_rate(channel.rate());

				}
			}

		}
		else
		{
			sprsp->set_error("LS_Err_NoStreamRelayService_Connected");
		}

	}
	else if (pRaw->networktype() == ChannelNetworkType::Remote)
	{
		auto spSession = GlobalManager::Instance()->GetTcpServer()->GetSessionByType(eLSMStreamRelay);
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

				}
			}


		}
		else
		{
			sprsp->set_error("LS_Err_NoStreamRelayService_Connected");

		}


	}


	spSockContainer->Send(sprsp);
}
