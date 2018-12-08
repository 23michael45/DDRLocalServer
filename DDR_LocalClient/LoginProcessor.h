#ifndef LoginProcessor_h__
#define LoginProcessor_h__


#include "../../Shared/src/Network/MessageSerializer.h"
#include "../../Shared/src/Network/BaseProcessor.h"


using namespace DDRFramework;
class LoginProcessor : public BaseProcessor
{
public:
	LoginProcessor();
	~LoginProcessor();

	virtual void Process(std::shared_ptr<TcpSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg);
};

#endif // LoginProcessor_h__
