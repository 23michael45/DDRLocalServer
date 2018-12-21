#ifndef AudioStreamAddrProcessor_h__
#define AudioStreamAddrProcessor_h__



#include "../../../Shared/src/Network/BaseProcessor.h"
#include "../../../Shared/src/Network/TcpSocketContainer.h"

class  AudioStreamAddrProcessor : public DDRFramework::BaseProcessor
{
public:
	AudioStreamAddrProcessor(DDRFramework::BaseMessageDispatcher& dispatcher);
	~AudioStreamAddrProcessor();


	virtual void Process(std::shared_ptr<DDRFramework::BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg) override;
};

#endif // AudioStreamAddrProcessor_h__
