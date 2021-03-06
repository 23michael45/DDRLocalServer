#ifndef SelectLSProcessor_h__
#define SelectLSProcessor_h__


#include "src/Network/MessageSerializer.h"
#include "src/Network/BaseProcessor.h"


using namespace DDRFramework;
class SelectLSProcessor : public BaseProcessor
{
public:
	SelectLSProcessor(BaseMessageDispatcher& dispatcher);
	~SelectLSProcessor();

	virtual void Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg) override;
	virtual void AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg) override;
private:
};

#endif // SelectLSProcessor_h__

