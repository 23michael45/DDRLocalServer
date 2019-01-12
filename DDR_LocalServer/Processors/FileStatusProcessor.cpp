
#include <memory>
#include "../../../Shared/proto/BaseCmd.pb.h"
#include "FileStatusProcessor.h"
#include "../../../Shared/src/Utility/DDRMacro.h"
#include "../../../Shared/src/Utility/Logger.h"
#include "../../../Shared/src/Network/TcpServerBase.h"
#include "../../../Shared/src/Network/HttpClient.h"
#include "../Managers/StreamRelayServiceManager.h"
#include "../Managers/GlobalManager.h"
#include "../Managers/FileManager.h"
#include "../Servers/LocalTcpServer.h"
#include "../LSClient/LSClientManager.h"
#include "../../../Shared/thirdparty/cppfs/include/cppfs/windows/LocalFileSystem.h"
#include "../../../Shared/thirdparty/cppfs/include/cppfs/FilePath.h"

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
}

void FileStatusProcessor::AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{



	ansFileStatus* pRaw = reinterpret_cast<ansFileStatus*>(spMsg.get());

	if (pRaw)
	{


		auto sprsp = std::make_shared<rspFileAddress>();

		auto spStreamRelaySession = std::dynamic_pointer_cast<LocalServerTcpSession>(spSockContainer->GetTcp());
		if (spStreamRelaySession)
		{
			sprsp->set_tarservicetype(spStreamRelaySession->GetLoginInfo().type());
			sprsp->set_filetype(pRaw->filetype());
			

			int toIntptr;
			eCltType nodetype;
			if (MsgRouterManager::Instance()->ReturnPassNode(spHeader, toIntptr, nodetype))
			{

				if (MsgRouterManager::Instance()->IsLastPassNode(spHeader))//msg is from local network
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

								for (auto file : pRaw->fileaddrlist())
								{
									sprsp->add_fileaddrlist(file);
								}


								spSession = spSessionPair.second;
								spSession->Send(spHeader, sprsp);
								break;
							}
						}
					}
				}
				else//msg is from remote network
				{

					//get file to local www
					for (auto file : pRaw->fileaddrlist())
					{
						if (!FileManager::Instance()->FileExist(file))
						{

							cppfs::FilePath fpath(file);
							auto filename = fpath.fileName();
							auto spHttpSession = std::make_shared<HttpSession>();
							spHttpSession->DoGet(file, FileManager::Instance()->HttpAddr2BaseDir(file));
						}
					}

				


					//Client Session Operation(To Remote Server)
					auto spClientSession = LSClientManager::Instance()->GetTcpClient()->GetConnectedSession();
					if (spClientSession)
					{
				

						int IntPtr = (int)(spClientSession.get());
						if (nodetype == eLocalServer)
						{
							if (IntPtr == toIntptr)
							{
								for (auto file : pRaw->fileaddrlist())
								{
									sprsp->add_fileaddrlist(FileManager::Instance()->GetRelativeDir(file));
								}

								spClientSession->Send(spHeader, sprsp);
							}
						}
					}
				}
			}
		}
	}
}
