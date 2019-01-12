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

			auto spSession = spSockContainer->GetTcp();
			MsgRouterManager::Instance()->RecordPassNode(spHeader, spSession);



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

			int toIntptr;
			eCltType nodetype;
			if (MsgRouterManager::Instance()->ReturnPassNode(spHeader, toIntptr, nodetype))
			{
				for (auto spSessionPair : map)
				{
					int IntPtr = (int)(spSessionPair.second.get());


					if (nodetype == eRemoteServer)
					{
						if (IntPtr == toIntptr)
						{
							spSession = spSessionPair.second;
							spSession->Send(spHeader, buf, bodylen);
							break;
						}
					}
				}
			}

		}



	}
	else
	{
		return false;
	};
}