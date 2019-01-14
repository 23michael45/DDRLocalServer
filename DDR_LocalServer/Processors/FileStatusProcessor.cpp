
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
	m_spHeader = spHeader;
	ansFileStatus* pRaw = reinterpret_cast<ansFileStatus*>(spMsg.get());

	if (pRaw)
	{

		if (MsgRouterManager::Instance()->ReturnPassNode(spHeader, m_Passnode))
		{


			auto spFromSession = std::dynamic_pointer_cast<LocalServerTcpSession>(spSockContainer->GetTcp());
			if (spFromSession)
			{
				for (auto file : pRaw->fileaddrlist())
				{
					if (!FileManager::Instance()->FileExist(file))
					{

						cppfs::FilePath fpath(file);
						auto filename = fpath.fileName();
						auto spHttpSession = std::make_shared<HttpSession>();
						spHttpSession->BindOnGetDoneFunc(std::bind(&FileStatusProcessor::OnGetDone, this, std::placeholders::_1));
						spHttpSession->DoGet(file, FileManager::Instance()->HttpAddr2BaseDir(file));
					}

				}
			}
		}
	}

	
}

void FileStatusProcessor::OnGetDone(float f)
{
	DebugLog("FileStatusProcessor OnGetDone %f", f);


	if (m_Passnode.intptrdata().size() > 0)
	{
		notifyUploadFile* pnotify = (notifyUploadFile*)(m_Passnode.intptrdata(0));
		if (pnotify)
		{
			std::set<std::string> remoteExistFiles;
			for (auto file : pnotify->existfiles())
			{
				remoteExistFiles.insert(file);
			}

			std::vector<std::string> matchedfiles;
			for (auto fmt : pnotify->filefmt())
			{
				auto files = FileManager::Instance()->Match(fmt);
				for (auto fullpath : files)
				{
					matchedfiles.push_back(FileManager::Instance()->GetFullDirFromRelative(fullpath));
				}
			}


			for (auto fileupload : matchedfiles)
			{
				if (remoteExistFiles.find(fileupload) != remoteExistFiles.end())//remote server exist
				{

				}
				else
				{
					std::string httpurl = pnotify->httpaddr();
					auto spHttpSession = std::make_shared<HttpSession>();
					spHttpSession->BindOnPostDoneFunc(std::bind(&FileStatusProcessor::OnPostDone, this, std::placeholders::_1));
					spHttpSession->DoPost(httpurl, FileManager::Instance()->GetRootPath(), fileupload);


				}

			}

			delete pnotify;
		}

	}


}
void FileStatusProcessor::OnPostDone(float f)
{
	DebugLog("FileStatusProcessor OnPostDone %f", f);

	auto sprsp = std::make_shared<notifyUploadFileProgress>();
	sprsp->set_progress(1.0f);


	//Server Session Operation;
	auto spSession = LSClientManager::Instance()->GetTcpClient()->GetConnectedSession();


	int IntPtr = (int)(spSession.get());
	if (m_Passnode.nodetype() == eLocalServer)
	{
		if (IntPtr == m_Passnode.receivesessionid())
		{
			spSession->Send(m_spHeader, sprsp);

		}
	}

	m_spHeader.reset();
}




void FileStatusProcessor::ProcessAns(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
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


			CommonHeader_PassNode passnode;
			if (MsgRouterManager::Instance()->ReturnPassNode(spHeader, passnode))
			{

				if (MsgRouterManager::Instance()->IsLastPassNode(spHeader))//msg is from local network
				{

					//Server Session Operation;
					auto map = GlobalManager::Instance()->GetTcpServer()->GetTcpSocketContainerMap();
					std::shared_ptr<TcpSessionBase> spSession = nullptr;

					for (auto spSessionPair : map)
					{
						int IntPtr = (int)(spSessionPair.second.get());
						if (passnode.nodetype() == eLocalServer)
						{
							if (IntPtr == passnode.receivesessionid())
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
						if (passnode.nodetype() == eLocalServer)
						{
							if (IntPtr == passnode.receivesessionid())
							{
								for (auto file : pRaw->fileaddrlist())
								{
									sprsp->add_fileaddrlist(file);
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

void FileStatusProcessor::ProcessChk(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{
	chkFileStatus* pRaw = reinterpret_cast<chkFileStatus*>(spMsg.get());

	if (pRaw)
	{


	}
}
