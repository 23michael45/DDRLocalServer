#ifndef RemoteFileAddressProcessor_h__
#define RemoteFileAddressProcessor_h__


#include "src/Network/MessageSerializer.h"
#include "src/Network/BaseProcessor.h"


using namespace DDRFramework;
class RemoteFileAddressProcessor : public BaseProcessor
{
public:
	RemoteFileAddressProcessor(BaseMessageDispatcher& dispatcher);
	~RemoteFileAddressProcessor();

	virtual void Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg) override;
	virtual void AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg) override;
private:

	void Lengancy(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg);

};

#endif // RemoteFileAddressProcessor_h__
