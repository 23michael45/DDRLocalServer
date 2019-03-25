#include "StreamAddrProcessor.h"
#include "../../../Shared/proto/BaseCmd.pb.h"
#include "../../../Shared/src/Utility/DDRMacro.h"
#include "../Managers/StreamProxyManager.h"
#include "../Servers/RemoteTcpServer.h"
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

	if (pRaw->networktype() == ChannelNetworkType::Remote)
	{

		auto spFromSession = dynamic_pointer_cast<RemoteServerTcpSession>(spSockContainer->GetTcp());
		if (spFromSession)
		{
			std::string udid = spFromSession->GetBindLSUDID();
			if (!udid.empty())
			{

				auto ChannelVec = StreamProxyManager::Instance()->GetRobotUploadAddr(udid);

				for (auto channel : ChannelVec)
				{
					auto pchannel = sprsp->add_channels();
					pchannel->CopyFrom(channel);

				}
			}
			else
			{

				sprsp->set_error("not bind ls udid");
			}

		}
		else
		{

			sprsp->set_error("session is not client");
		}


	}
	else
	{
		sprsp->set_error("Net Type Error");

	}


	spSockContainer->Send(sprsp);
}
