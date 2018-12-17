#ifndef FileProcessor_h__
#define FileProcessor_h__



#include "../../../Shared/src/Network/BaseProcessor.h"
#include "../../../Shared/src/Network/TcpSocketContainer.h"

class  FileProcessor : public DDRFramework::BaseProcessor
{
public:
	FileProcessor(DDRFramework::BaseMessageDispatcher& dispatcher);
	~FileProcessor();


	virtual void Process(std::shared_ptr<DDRFramework::BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg) override;
};


#endif // FileProcessor_h__
