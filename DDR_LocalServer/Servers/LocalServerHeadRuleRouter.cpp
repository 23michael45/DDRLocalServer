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

bool LocalServerHeadRuleRouter::IgnoreBody(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader,asio::streambuf& buf, int bodylen)
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
				pNode->set_nodetype(eLocalServer);

				auto spSession = spSockContainer->GetTcp();
				pNode->set_receivesessionid((int)spSession.get());



				//Client Session Operation(To Remote Server)
				auto spClientSession = LSClientManager::Instance()->GetTcpClient()->GetConnectedSession();
				if (spClientSession)
				{
				}
				else//Server Session Operation;
				{
					auto map = GlobalManager::Instance()->GetTcpServer()->GetTcpSocketContainerMap();

					bool hasSession = false;
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
					if (hasSession == false)
					{
						DebugLog("No Dest Session Conncected:%i", toType);
					}
				}

			}
			else if (spHeader->flowdirection(0) == CommonHeader_eFlowDir_Backward)
			{

				auto passnodes = spHeader->mutable_passnode();

				if (passnodes->size() > 0)
				{
					google::protobuf::RepeatedPtrField<CommonHeader_PassNode>::iterator it = passnodes->begin();


					//Client Session Operation(To Remote Server)
					auto spClientSession = LSClientManager::Instance()->GetTcpClient()->GetConnectedSession();
					if (spClientSession)
					{
						int IntPtr = (int)(spClientSession.get());
						if (it->nodetype() == eLocalServer)
						{
							if (IntPtr == it->receivesessionid())
							{
								spClientSession->Send(spHeader, buf, bodylen);
							}
						}
					}
					else//Server Session Operation;
					{
						auto map = GlobalManager::Instance()->GetTcpServer()->GetTcpSocketContainerMap();
						std::shared_ptr<TcpSessionBase> spSession = nullptr;

						for (auto spSessionPair : map)
						{
							int IntPtr = (int)(spSessionPair.second.get());
							if (it->nodetype() == eLocalServer)
							{
								if (IntPtr == it->receivesessionid())
								{
									spSession = spSessionPair.second;
									break;
								}
							}
						}


						if (spSession && it != passnodes->end())
						{

							spSession->Send(spHeader, buf, bodylen);

						}
					}



					passnodes->erase(it);
				}
			}


		}
	}
	else
	{
		return false;
	}

	
	
	
};