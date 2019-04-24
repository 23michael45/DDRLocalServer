#ifndef UnselectLSProcessor_h__
#define UnselectLSProcessor_h__


#include "src/Network/MessageSerializer.h"
#include "src/Network/BaseProcessor.h"


using namespace DDRFramework;
class UnselectLSProcessor : public BaseProcessor
{
public:
	UnselectLSProcessor(BaseMessageDispatcher& dispatcher);
	~UnselectLSProcessor();

	virtual void Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg) override;
	virtual void AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg) override;
private:
};

#endif // UnselectLSProcessor_h__
