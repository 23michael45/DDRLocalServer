#include "LocalServerHeadRuleRouter.h"
#include "../Managers/StreamRelayServiceManager.h"
#include "../Managers/GlobalManager.h"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/repeated_field.h>


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
		//if forward , record the route node
		if (spHeader->flowdirection().size() > 0 && spHeader->flowdirection(0) == CommonHeader_eFlowDir_Forward)
		{
			CommonHeader_PassNode* pNode = spHeader->add_passnode();
			pNode->set_nodetype(eLocalServer);

			auto spSession = spSockContainer->GetTcp();
			pNode->set_receivesessionid((int)spSession.get());

		}


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

				std::shared_ptr<TcpSessionBase> spSession = nullptr;

				auto passnodes = spHeader->mutable_passnode();
				google::protobuf::RepeatedPtrField<CommonHeader_PassNode>::iterator it = passnodes->end();

				for (auto spSessionPair : map)
				{
					int IntPtr = (int)(spSessionPair.second.get());


					for (it = passnodes->begin(); it != passnodes->end(); it++)
					{
						if (it->nodetype() == eLocalServer)
						{
							if (IntPtr == it->receivesessionid())
							{
								spSession = spSessionPair.second;
								break;
							}
						}
					}

					if (spSession)
					{
						break;
					}
				}


				if (spSession && it != passnodes->end())
				{
					auto passnodes = spHeader->mutable_passnode();
					passnodes->erase(it);

					spSession->Send(spHeader, buf, bodylen);

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