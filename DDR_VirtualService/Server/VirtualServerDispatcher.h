#ifndef VirtualServerDispatcher_h__
#define VirtualServerDispatcher_h__



#include "src/Network/BaseMessageDispatcher.h"
#include "src/Network/BaseSocketContainer.h"


class VirtualServerDispatcher : public DDRFramework::BaseMessageDispatcher
{
public:
	VirtualServerDispatcher();
	~VirtualServerDispatcher();


	virtual void Dispatch(std::shared_ptr< DDRFramework::BaseSocketContainer> spParentSocketContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg) override;

};

#endif // VirtualServerDispatcher_h__
