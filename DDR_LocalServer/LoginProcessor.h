#ifndef LoginProcessor_h__
#define LoginProcessor_h__

#include "../../Shared/src/Network/BaseProcessor.h"
#include "../../Shared/src/Network/TcpSocketContainer.h"



class LoginProcessor : public DDRFramework::BaseProcessor
{
public:
	LoginProcessor();
	~LoginProcessor();


	virtual void Process(std::shared_ptr<DDRFramework::TcpSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg);
};


#endif // LoginProcessor_h__