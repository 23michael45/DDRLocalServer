#include "LocalServerHeadRuleRouter.h"
#include "../Managers/StreamRelayServiceManager.h"
#include "../Managers/GlobalManager.h"
#include "../LSClient/LSClientManager.h"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/repeated_field.h>


LocalServerHeadRuleRouter::LocalServerHeadRuleRouter()
{
}


LocalServerHeadRuleRouter::~LocalServerHeadRuleRouter()
{
}

bool LocalServerHeadRuleRouter::IgnoreBody(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, asio::streambuf& buf, int bodylen)
{
	if (spHeader->flowdirection().size() > 0)
	{
		//if forward , record the route node
		if (spHeader->flowdirection(0) == CommonHeader_eFlowDir_Forward)
		{

			eCltType toType = spHeader->toclttype();

			CommonHeader_PassNode* pNode = spHeader->add_passnodearray();
			pNode->set_nodetype(eLocalServer);

			auto spSession = spSockContainer->GetTcp();
			pNode->set_receivesessionid((int)spSession.get());


			bool hasSession = false;

			//do Local forward operation
			auto map = GlobalManager::Instance()->GetTcpServer()->GetTcpSocketContainerMap();

			for (auto pair : map)
			{

				auto spSession = pair.second;
				auto spServerSessionTcp = dynamic_pointer_cast<LocalServerTcpSession>(spSession);

				auto sessiontype = spServerSessionTcp->GetLoginInfo().type();
				if ((sessiontype & toType) != 0)
				{
					spServerSessionTcp->Send(spHeader, buf, bodylen);
					hasSession = true;
				}
			}

			//Client Session Operation(To Remote Server)
			if ((eAllClient & toType) != 0)
			{
				auto spClientSession = LSClientManager::Instance()->GetTcpClient()->GetConnectedSession();
				if (spClientSession)
				{
					spClientSession->Send(spHeader, buf, bodylen);
				}
			}


			if (hasSession == false)
			{
				DebugLog("No Dest Session Conncected:%i", toType);
			}

		}
		else if (spHeader->flowdirection(0) == CommonHeader_eFlowDir_Backward)
		{

			auto passnodes = spHeader->mutable_passnodearray();

			if (passnodes->size() > 0)
			{
				google::protobuf::RepeatedPtrField<CommonHeader_PassNode>::reverse_iterator rit = passnodes->rbegin();


				//Client Session Operation(To Remote Server)
				auto spClientSession = LSClientManager::Instance()->GetTcpClient()->GetConnectedSession();
				if (spClientSession)
				{
					int IntPtr = (int)(spClientSession.get());
					if (rit->nodetype() == eLocalServer)
					{
						if (IntPtr == rit->receivesessionid())
						{
							spClientSession->Send(spHeader, buf, bodylen);
						}
					}
				}


				//Server Session Operation;
				auto map = GlobalManager::Instance()->GetTcpServer()->GetTcpSocketContainerMap();
				std::shared_ptr<TcpSessionBase> spSession = nullptr;

				for (auto spSessionPair : map)
				{
					int IntPtr = (int)(spSessionPair.second.get());
					if (rit->nodetype() == eLocalServer)
					{
						if (IntPtr == rit->receivesessionid())
						{
							spSession = spSessionPair.second;
							spSession->Send(spHeader, buf, bodylen);
							break;
						}
					}
				}


				if (spSession && rit != passnodes->rend())
				{
					passnodes->erase(rit.base());
				}




			}
		}
	}
	else
	{
		return false;
	}

};