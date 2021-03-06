#ifndef RegisteLS_h__
#define RegisteLS_h__


#include "src/Network/MessageSerializer.h"
#include "src/Network/BaseProcessor.h"


using namespace DDRFramework;
class RegisteLSProcessor : public BaseProcessor
{
public:
	RegisteLSProcessor(BaseMessageDispatcher& dispatcher);
	~RegisteLSProcessor();

	virtual void Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg) override;
	virtual void AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg) override;
private:
};

#endif // RegisteLS_h__
