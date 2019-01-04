#include "LocalServerHeadRuleRouter.h"
#include "../Managers/StreamRelayServiceManager.h"
#include "../Managers/GlobalManager.h"


LocalServerHeadRuleRouter::LocalServerHeadRuleRouter()
{
}


LocalServerHeadRuleRouter::~LocalServerHeadRuleRouter()
{
}

bool LocalServerHeadRuleRouter::IgnoreBody(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader,asio::streambuf& buf, int bodylen)
{
	auto spHeaderReg = MsgRouterManager::Instance()->FindCommonHeader(spHeader->bodytype());
	if (spHeaderReg)
	{
		CommonHeader_PassNode* pNode = 	spHeader->add_passnode();
		pNode->set_nodetype(eLocalServer);

		auto spSession = spSockContainer->GetTcp();
		pNode->set_receivesessionid((int)spSession.get());

		if (spHeader->toclttype().size() > 0)
		{
			eCltType toType = spHeader->toclttype(0);

			if (toType == eLSMStreamRelay)
			{
				auto spStreamRelaySession = StreamRelayServiceManager::Instance()->GetServerSession();
				if (spStreamRelaySession)
				{
					StreamRelayServiceManager::Instance()->Send(spHeader,buf,bodylen);

				}
				else
				{
					DebugLog("No StreamRelayServer Conncected");
				}

			}
			else if (toType == eLSMFaceRecognition)
			{

			}
			else if (toType == eLSMSlamNavigation)
			{

			}
			else if (toType == eLSMThermalImaging)
			{

			}
			else if((toType & eAllClient) != 0)
			{
				auto map = GlobalManager::Instance()->GetTcpServer()->GetTcpSocketContainerMap();

				for (auto spSession : map)
				{
					int IntPtr = (int)(spSession.second.get());

					if (IntPtr == spHeader->passnode(0).receivesessionid())
					{
						spSession.second->Send(spHeader,buf,bodylen);
					}
				}

			}

		}

		return true;
	}
	else
	{

		return false;
	}
	
};