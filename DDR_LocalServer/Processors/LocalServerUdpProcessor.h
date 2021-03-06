#ifndef LocadServerUdpProcessor_h__
#define LocadServerUdpProcessor_h__



#include "src/Network/BaseProcessor.h"
#include "src/Network/TcpSocketContainer.h"
#include "src/Network/BaseSocketContainer.h"



class LocalServerUdpProcessor : public DDRFramework::BaseProcessor
{
public:
	LocalServerUdpProcessor(DDRFramework::BaseMessageDispatcher& dispatcher);
	~LocalServerUdpProcessor();


	virtual void Process(std::shared_ptr<DDRFramework::BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg) override;

};


#endif // LocadServerUdpProcessor_h__