#ifndef RemoteServerDispatcher_h__
#define RemoteServerDispatcher_h__


#include "src/Network/BaseMessageDispatcher.h"


class RemoteServerDispatcher : public DDRFramework::BaseMessageDispatcher
{
public:
	RemoteServerDispatcher();
	~RemoteServerDispatcher();


	virtual void Dispatch(std::shared_ptr< DDRFramework::BaseSocketContainer> spParentSocketContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg) override;
};


#endif // RemoteServerDispatcher_h__