#ifndef FileAddressProcessor_h__
#define FileAddressProcessor_h__



#include "../../../Shared/src/Network/BaseProcessor.h"
#include "../../../Shared/src/Network/TcpSocketContainer.h"

class  FileAddressProcessor : public DDRFramework::BaseProcessor
{
public:
	FileAddressProcessor(DDRFramework::BaseMessageDispatcher& dispatcher);
	~FileAddressProcessor();


	virtual void Process(std::shared_ptr<DDRFramework::BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg) override;
};


#endif // FileAddressProcessor_h__
