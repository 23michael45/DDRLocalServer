#include "RemoteServerDispatcher.h"

#include "../../../Shared/proto/BaseCmd.pb.h"
#include "../Processors/RemoteServerListProcessor.h"
#include "../Processors/RemoteLoginProcessor.h"
#include "../Processors/SelectLSProcessor.h"
#include "../Processors/RegisteLSProcessor.h"
#include "../Processors/RemoteFileAddressProcessor.h"
#include "../Processors/FileStatusProcessor.h"
#include "../Processors/UploadFileProgressProcessor.h"
#include "../Processors/RtspStreamUploadAddrProcessor.h"
#include "../Processors/StreamAddrProcessor.h"
#include "../Servers/RemoteTcpServer.h"

using namespace DDRCommProto;
using namespace DDRFramework;

RemoteServerDispatcher::RemoteServerDispatcher()
{

	//Global
	RegisterProcessor(req, RegisteLS)
	RegisterProcessor(req, RemoteServerList)
		RegisterProcessor(req, RemoteLogin)
		RegisterProcessor(req, SelectLS)


		RegisterProcessor(req, RemoteFileAddress)
		RegisterProcessor(ans, FileStatus)

		RegisterProcessor(notify, UploadFileProgress)

		RegisterProcessor(req, RtspStreamUploadAddr)

		RegisterProcessor(req, StreamAddr)


}


RemoteServerDispatcher::~RemoteServerDispatcher()
{
}


void RemoteServerDispatcher::Dispatch(std::shared_ptr< DDRFramework::BaseSocketContainer> spParentSocketContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{
	auto spServerTcpSession = dynamic_pointer_cast<RemoteServerTcpSession>(spParentSocketContainer->GetTcp());

	if (spServerTcpSession->HasLogin() || spHeader->bodytype() == "DDRCommProto.reqRemoteLogin")
	{

		BaseMessageDispatcher::Dispatch(spParentSocketContainer, spHeader, spMsg);
	}
	else
	{
		if (spServerTcpSession->GetType() == RemoteServerTcpSession::RST_CLIENT)
		{
			DebugLog("Dispatch Error ,Client  has not login ")

		}
		else if(spServerTcpSession->GetType() == RemoteServerTcpSession::RST_LS)
		{

			DebugLog("Dispatch Error ,  LS has not registed ")
		}
	}

}

