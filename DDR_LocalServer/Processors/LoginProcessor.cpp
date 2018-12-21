#include "LoginProcessor.h"
#include <memory>
#include "../../../Shared/proto/BaseCmd.pb.h"
#include "../../../Shared/src/Utility/DDRMacro.h"
#include "../Behaviors/BaseClientBehavior.h"
#include "../Managers/GlobalManager.h"
using namespace DDRFramework;
using namespace DDRCommProto;

LoginProcessor::LoginProcessor(BaseMessageDispatcher& dispatcher):BaseProcessor::BaseProcessor(dispatcher)
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
	std::string pwd = pRaw->userpwd();
	eCltType type = pRaw->type();


	auto sprsp = std::make_shared<rspLogin>();

	bool closeSession = false;
	if (type == eCltType::ePCClient)
	{
		bool b = DBManager::Instance()->VerifyUser(name, pwd);
		if (b)
		{

			sprsp->set_retcode(rspLogin_eLoginRetCode_success);
		}
		else
		{
			sprsp->set_retcode(rspLogin_eLoginRetCode_incorrect_password);
		
			closeSession = true;
		}
	}
	else if (type == eCltType::eAndroidClient)
	{
		bool b = DBManager::Instance()->VerifyUser(name, pwd);
		if (b)
		{

			sprsp->set_retcode(rspLogin_eLoginRetCode_success);
		}
		else
		{
			sprsp->set_retcode(rspLogin_eLoginRetCode_incorrect_password);		
	
			closeSession = true;
		}

	}
	else if (type == eCltType::eLSMStreamRelay)
	{

		sprsp->set_retcode(rspLogin_eLoginRetCode_success);
	}
	else if (type == eCltType::eLSMFaceRecognition)
	{


		sprsp->set_retcode(rspLogin_eLoginRetCode_success);

	}
	else if (type == eCltType::eLSMSlamNavigation)
	{



		sprsp->set_retcode(rspLogin_eLoginRetCode_success);
	}
	else if (type == eCltType::eLSMThermalImaging)
	{


		sprsp->set_retcode(rspLogin_eLoginRetCode_success);

	}


	spSockContainer->Send(sprsp);
	if (closeSession)
	{
		spSockContainer->GetTcp()->Stop();
	}
	else
	{ 

		auto spClientBehavior = std::make_shared<BaseClientBehavior>();
		spSockContainer->m_spTcpSocketContainer->BindBehavior(spClientBehavior);


		auto spTcp = spSockContainer->GetTcp();
		if (spTcp)
		{
			auto spServerSessionTcp = dynamic_pointer_cast<TcpSessionBase>(spTcp);
			if (spServerSessionTcp)
			{
				spServerSessionTcp->AssignLoginInfo(*pRaw);
			}
		}
	}



	DebugLog("\nLogin %s:" ,name.c_str());


}
void LoginProcessor::AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer,std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{
	//std::this_thread::sleep_for(std::chrono::seconds(10));
}