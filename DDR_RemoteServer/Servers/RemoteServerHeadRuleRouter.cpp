#include "RemoteServerHeadRuleRouter.h"
#include "../Managers/GlobalManager.h"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/repeated_field.h>


RemoteServerHeadRuleRouter::RemoteServerHeadRuleRouter()
{
}


RemoteServerHeadRuleRouter::~RemoteServerHeadRuleRouter()
{
}

bool RemoteServerHeadRuleRouter::IgnoreBody(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, asio::streambuf& buf, int bodylen)
{
	if (spHeader->flowdirection().size() > 0)
	{	//if forward , record the route node
		if (spHeader->flowdirection(0) == CommonHeader_eFlowDir_Forward)
		{

			eCltType toType = spHeader->toclttype();

			CommonHeader_PassNode* pNode = spHeader->add_passnodearray();
			pNode->set_nodetype(eRemoteServer);

			auto spSession = spSockContainer->GetTcp();
			pNode->set_receivesessionid((int)spSession.get());


			auto spFromSession = dynamic_pointer_cast<RemoteServerTcpSession>(spSockContainer->GetTcp());
			if (spFromSession)
			{

				if (spFromSession->GetType() == RemoteServerTcpSession::RemoteServerTcpSessionType::RST_CLIENT)
				{
					std::string udid = spFromSession->GetBindLSUDID();
					auto spTo = GlobalManager::Instance()->GetTcpServer()->GetLSSession(udid);
					if (spTo)
					{
						spTo->Send(spHeader, buf, bodylen);
					}
					else
					{
						DebugLog("Client Controls Nothing");

					}

				}
				else if (spFromSession->GetType() == RemoteServerTcpSession::RemoteServerTcpSessionType::RST_LS)
				{
					auto map = GlobalManager::Instance()->GetTcpServer()->GetClientMap();
					for (auto pair : map)
					{
						auto spClientSession = pair.second;
						spClientSession->Send(spHeader, buf, bodylen);

					}


				}
				
				


			}
			else
			{
				DebugLog("RemoteServerHeadRuleRouter Route Error");
			}
		}
		else if (spHeader->flowdirection(0) == CommonHeader_eFlowDir_Backward)
		{


			auto map = GlobalManager::Instance()->GetTcpServer()->GetTcpSocketContainerMap();

			std::shared_ptr<TcpSessionBase> spSession = nullptr;

			auto passnodes = spHeader->mutable_passnodearray();
			google::protobuf::RepeatedPtrField<CommonHeader_PassNode>::iterator it = passnodes->end();

			for (auto spSessionPair : map)
			{
				int IntPtr = (int)(spSessionPair.second.get());


				for (it = passnodes->begin(); it != passnodes->end(); it++)
				{
					if (it->nodetype() == eRemoteServer)
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
				auto passnodes = spHeader->mutable_passnodearray();
				passnodes->erase(it);

				spSession->Send(spHeader, buf, bodylen);

			}

		}



	}
	else
	{
		return false;
	};
}