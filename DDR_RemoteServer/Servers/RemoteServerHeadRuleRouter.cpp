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
	{
		//auto spHeaderReg = MsgRouterManager::Instance()->FindCommonHeader(spHeader->bodytype());
		if (false)
		{
			////if forward , record the route node
			//if (spHeader->flowdirection().size() > 0 && spHeader->flowdirection(0) == CommonHeader_eFlowDir_Forward)
			//{
			//	CommonHeader_PassNode* pNode = spHeader->add_passnode();
			//	pNode->set_nodetype(eLocalServer);

			//	auto spSession = spSockContainer->GetTcp();
			//	pNode->set_receivesessionid((int)spSession.get());

			//}


			//if (spHeader->toclttype().size() > 0)
			//{
			//	eCltType toType = spHeader->toclttype(0);

			//	if (toType == eLSMStreamRelay)
			//	{
			//		auto spStreamRelaySession = StreamRelayServiceManager::Instance()->GetServerSession();
			//		if (spStreamRelaySession)
			//		{
			//			StreamRelayServiceManager::Instance()->Send(spHeader, buf, bodylen);

			//		}
			//		else
			//		{
			//			DebugLog("No eLSMStreamRelay Conncected");
			//		}

			//	}
			//	else if (toType == eLSMFaceRecognition)
			//	{

			//	}
			//	else if (toType == eLSMSlamNavigation)
			//	{
			//		auto map = GlobalManager::Instance()->GetTcpServer()->GetTcpSocketContainerMap();

			//		bool hasSession = false;
			//		for (auto pair : map)
			//		{

			//			auto spSession = pair.second;
			//			auto spServerSessionTcp = dynamic_pointer_cast<LocalServerTcpSession>(spSession);
			//			if (spServerSessionTcp->GetLoginInfo().type() == toType)
			//			{
			//				spServerSessionTcp->Send(spHeader, buf, bodylen);
			//				hasSession = true;
			//				break;
			//			}
			//		}
			//		if (hasSession == false)
			//		{

			//			DebugLog("No eLSMSlamNavigation Conncected");
			//		}



			//	}
			//	else if (toType == eLSMThermalImaging)
			//	{

			//	}
			//	else if ((toType & eAllClient) != 0)
			//	{
			//		auto map = GlobalManager::Instance()->GetTcpServer()->GetTcpSocketContainerMap();

			//		std::shared_ptr<TcpSessionBase> spSession = nullptr;

			//		auto passnodes = spHeader->mutable_passnode();
			//		google::protobuf::RepeatedPtrField<CommonHeader_PassNode>::iterator it = passnodes->end();

			//		for (auto spSessionPair : map)
			//		{
			//			int IntPtr = (int)(spSessionPair.second.get());


			//			for (it = passnodes->begin(); it != passnodes->end(); it++)
			//			{
			//				if (it->nodetype() == eLocalServer)
			//				{
			//					if (IntPtr == it->receivesessionid())
			//					{
			//						spSession = spSessionPair.second;
			//						break;
			//					}
			//				}
			//			}

			//			if (spSession)
			//			{
			//				break;
			//			}
			//		}


			//		if (spSession && it != passnodes->end())
			//		{
			//			auto passnodes = spHeader->mutable_passnode();
			//			passnodes->erase(it);

			//			spSession->Send(spHeader, buf, bodylen);

			//		}

			//	}

			//}

			return true;
		}
		else
		{	//if forward , record the route node
			if (spHeader->flowdirection(0) == CommonHeader_eFlowDir_Forward)
			{

				eCltType toType = spHeader->toclttype(0);

				CommonHeader_PassNode* pNode = spHeader->add_passnode();
				pNode->set_nodetype(eRemoteServer);

				auto spSession = spSockContainer->GetTcp();
				pNode->set_receivesessionid((int)spSession.get());



				auto map = GlobalManager::Instance()->GetTcpServer()->GetTcpSocketContainerMap();

				bool hasSession = false;
				for (auto pair : map)
				{

					auto spSession = pair.second;
					auto spServerSessionTcp = dynamic_pointer_cast<RemoteServerTcpSession>(spSession);


					if (spServerSessionTcp->GetType() == RemoteServerTcpSession::RemoteServerTcpSessionType::RST_CLIENT)
					{
						std::string udid = spServerSessionTcp->GetBindLSUDID();
						auto spTo = GlobalManager::Instance()->GetTcpServer()->GetLSSession(udid);
						if (spTo)
						{

							spTo->Send(spHeader, buf, bodylen);
							hasSession = true;
						}

					}
					else if (spServerSessionTcp->GetType() == RemoteServerTcpSession::RemoteServerTcpSessionType::RST_LS)
					{
						//to do to control client
						auto& map = spServerSessionTcp->GetBindClientMap();
						for (auto pair : map)
						{
							auto spClientSession = pair.second;
							spClientSession->Send(spHeader, buf, bodylen);

							hasSession = true;
						}
					}


				}
				if (hasSession == false)
				{
					DebugLog("No Dest Session Conncected:%i", toType);
				}


			}
			else if (spHeader->flowdirection(0) == CommonHeader_eFlowDir_Backward)
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
					auto passnodes = spHeader->mutable_passnode();
					passnodes->erase(it);

					spSession->Send(spHeader, buf, bodylen);

				}

			}


		}
	}
	else
	{
		return false;
	};
}