#include "LocalClientUdpProcessor.h"
#include <memory>
#include "../../Shared/proto/BaseCmd.pb.h"
#include "../../Shared/src/Utility/DDRMacro.h"
using namespace DDRFramework;
using namespace DDRCommProto;

LocalClientUdpProcessor::LocalClientUdpProcessor()
{
}


LocalClientUdpProcessor::~LocalClientUdpProcessor()
{
}

void LocalClientUdpProcessor::Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{
	auto bodytype = spHeader->bodytype();

	reqLogin* pRaw = reinterpret_cast<reqLogin*>(spMsg.get());

	std::string name = pRaw->username();


	auto sprsp = std::make_shared<respLogin>();
	sprsp->set_retcode(respLogin_eLoginRetCode_success);

	spSockContainer->Send(sprsp);

	//DebugLog("\nLogin %s:" ,name.c_str());


}
void LocalClientUdpProcessor::AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{

}