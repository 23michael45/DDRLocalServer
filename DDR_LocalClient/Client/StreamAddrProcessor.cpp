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


	if (pRaw->channels().size() > 0)
	{
		for (auto channel : pRaw->channels())
		{
			std::string ip = channel.dst();

			if (channel.dstport().size() > 0)
			{

				int port = channel.dstport(0);
				GlobalManager::Instance()->StartAudioClient(ip, port);
			}
			break;;
		}
	}
	else
	{
		DebugLog("\nNo StreamRelayServer Listening");
	}


	

}
