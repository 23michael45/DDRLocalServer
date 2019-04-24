#ifndef LocadClientUdpProcessor_h__
#define LocadClientUdpProcessor_h__

#ifdef QT_PRECOMPILED_HEADER
    #include "stable.h"
#else
#endif


#include "proto/BaseCmd.pb.h"
#include "src/Network/BaseProcessor.h"
#include "src/Network/TcpSocketContainer.h"

using namespace DDRCommProto;


class LocalClientUdpProcessor : public DDRFramework::BaseProcessor
{
public:
	LocalClientUdpProcessor(DDRFramework::BaseMessageDispatcher& dispatcher);
	~LocalClientUdpProcessor();


	virtual void Process(std::shared_ptr<DDRFramework::BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg) override;
private:
	void DealLocalServer(bcLSAddr_ServerInfo& serverinfo);

};


#endif // LocadClientUdpProcessor_h__
