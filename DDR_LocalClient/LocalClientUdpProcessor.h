#ifndef LocadClientUdpProcessor_h__
#define LocadClientUdpProcessor_h__



#include "../../Shared/src/Network/BaseProcessor.h"
#include "../../Shared/src/Network/TcpSocketContainer.h"



class LocalClientUdpProcessor : public DDRFramework::BaseProcessor
{
public:
	LocalClientUdpProcessor();
	~LocalClientUdpProcessor();


	virtual void Process(std::shared_ptr<DDRFramework::BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg) override;

	virtual void AsyncProcess(std::shared_ptr<DDRFramework::BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg) override;
};


#endif // LocadClientUdpProcessor_h__