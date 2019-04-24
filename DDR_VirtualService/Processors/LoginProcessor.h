#ifndef LoginProcessor_h__
#define LoginProcessor_h__


#include "src/Network/MessageSerializer.h"
#include "src/Network/BaseProcessor.h"


using namespace DDRFramework;
class LoginProcessor : public BaseProcessor
{
public:
	LoginProcessor(BaseMessageDispatcher& dispatcher);
	~LoginProcessor();

	virtual void Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg) override;

private:
};

#endif // LoginProcessor_h__
