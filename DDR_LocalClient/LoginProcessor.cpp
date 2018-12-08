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

void LoginProcessor::Process(std::shared_ptr<TcpSocketContainer> spSockContainer, std::shared_ptr<CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{

	respLogin* pRaw = reinterpret_cast<respLogin*>(spMsg.get());


	respLogin_eLoginRetCode retcode = pRaw->retcode();


	//DebugLog("\nLogin :%i", retcode);
}