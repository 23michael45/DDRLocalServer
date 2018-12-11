#include "LoginProcessor.h"
#include <memory>
#include "../../Shared/proto/BaseCmd.pb.h"
#include "../../Shared/src/Utility/DDRMacro.h"
using namespace DDRFramework;
using namespace DDRCommProto;

LoginProcessor::LoginProcessor()
{
}


LoginProcessor::~LoginProcessor()
{
}

void LoginProcessor::Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{
	auto bodytype = spHeader->bodytype();

	reqLogin* pRaw = reinterpret_cast<reqLogin*>(spMsg.get());

	std::string name = pRaw->username();


	auto sprsp = std::make_shared<respLogin>();
	sprsp->set_retcode(respLogin_eLoginRetCode_success);

	spSockContainer->Send(sprsp);



	//DebugLog("\nLogin %s:" ,name.c_str());


}
void LoginProcessor::AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{
	std::this_thread::sleep_for(std::chrono::seconds(10000));
	
}