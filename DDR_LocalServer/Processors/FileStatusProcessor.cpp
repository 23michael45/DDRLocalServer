
#include <memory>
#include "../../../Shared/proto/BaseCmd.pb.h"
#include "FileStatusProcessor.h"
#include "../../../Shared/src/Utility/DDRMacro.h"
#include "../../../Shared/src/Utility/Logger.h"
#include "../../../Shared/src/Network/TcpServerBase.h"
#include "../Managers/StreamRelayServiceManager.h"
#include "../Managers/GlobalManager.h"
#include "../Servers/LocalTcpServer.h"
#include "../LSClient/LSClientManager.h"

using namespace DDRFramework;
using namespace DDRCommProto;

FileStatusProcessor::FileStatusProcessor(BaseMessageDispatcher& dispatcher) :BaseProcessor(dispatcher)
{
}


FileStatusProcessor::~FileStatusProcessor()
{
}

void FileStatusProcessor::Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{

	ansFileStatus* pRaw = reinterpret_cast<ansFileStatus*>(spMsg.get());



	auto sprsp = std::make_shared<rspFileAddress>();

	auto spStreamRelaySession = std::dynamic_pointer_cast<LocalServerTcpSession>(spSockContainer->GetTcp());
	if (spStreamRelaySession)
	{
		sprsp->set_tarservicetype(spStreamRelaySession->GetLoginInfo().type());
		sprsp->set_filetype(pRaw->filetype());
		for (auto file : pRaw->fileaddrlist())
		{
			sprsp->add_fileaddrlist(file);
		}


		int toIntptr;
		eCltType nodetype;
		if (MsgRouterManager::Instance()->ReturnPassNode(spHeader, toIntptr, nodetype))
		{

			if (MsgRouterManager::Instance()->IsLastPassNode(spHeader))
			{
				//Server Session Operation;
				auto map = GlobalManager::Instance()->GetTcpServer()->GetTcpSocketContainerMap();
				std::shared_ptr<TcpSessionBase> spSession = nullptr;

				for (auto spSessionPair : map)
				{
					int IntPtr = (int)(spSessionPair.second.get());
					if (nodetype == eLocalServer)
					{
						if (IntPtr == toIntptr)
						{
							spSession = spSessionPair.second;
							spSession->Send(spHeader, sprsp);
							break;
						}
					}
				}
			}
			else
			{

				//Client Session Operation(To Remote Server)
				auto spClientSession = LSClientManager::Instance()->GetTcpClient()->GetConnectedSession();
				if (spClientSession)
				{
					int IntPtr = (int)(spClientSession.get());
					if (nodetype == eLocalServer)
					{
						if (IntPtr == toIntptr)
						{
							spClientSession->Send(spHeader, sprsp);
						}
					}
				}
			}



		}

		//do send to client

		/*std::map<std::shared_ptr<TcpSessionBase>, std::shared_ptr<TcpSessionBase>>& map = StreamRelayServiceManager::Instance()->m_WaitingSessionPare;
		if (map.find(spStreamRelaySession) != map.end() && map[spStreamRelaySession])
		{
			auto spClientSession = map[spStreamRelaySession];
			spClientSession->Send(sprsp);
			map.erase(spStreamRelaySession);
		}*/

	}

}

void FileStatusProcessor::AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{


}
