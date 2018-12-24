#include "StreamAddrProcessor.h"
#include "../../../Shared/proto/BaseCmd.pb.h"
#include "../../../Shared/src/Utility/DDRMacro.h"
#include "GlobalManager.h"
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


	std::string error = pRaw->error();
	if (error.empty())
	{

		if (pRaw->channels().size() > 0)
		{
			for (auto channel : pRaw->channels())
			{
				if (channel.networktype() == ChannelNetworkType::Local)
				{
					if (channel.srcport().size() > 0)
					{

						std::string ip = channel.srcaddr();
						int port = channel.srcport(0);
						GlobalManager::Instance()->StartAudioClient(ip, port);

					}
					else
					{
						//to do get 
					}


				}
				else if (channel.networktype() == ChannelNetworkType::Remote)
				{

				}

			}
		}
		else
		{
		}

	}
	else
	{
		auto serror = GlobalManager::Instance()->GetLocalizationConfig().GetString(error);
		DebugLog("\n%s", serror.c_str());
	}




	

}