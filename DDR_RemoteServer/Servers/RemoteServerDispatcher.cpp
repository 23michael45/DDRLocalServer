#include "RemoteServerDispatcher.h"

#include "../../../Shared/proto/BaseCmd.pb.h"
#include "../Processors/RemoteServerListProcessor.h"
#include "../Processors/RemoteLoginProcessor.h"
#include "../Processors/SelectLSProcessor.h"
#include "../Processors/RegisteLSProcessor.h"
#include "../Processors/RemoteFileAddressProcessor.h"
#include "../Processors/FileStatusProcessor.h"
#include "../Processors/UploadFileProgressProcessor.h"

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


}


RemoteServerDispatcher::~RemoteServerDispatcher()
{
}
