#ifndef RemoteServerDispatcher_h__
#define RemoteServerDispatcher_h__


#include "../../../Shared/src/Network/BaseMessageDispatcher.h"


class RemoteServerDispatcher : public DDRFramework::BaseMessageDispatcher
{
public:
	RemoteServerDispatcher();
	~RemoteServerDispatcher();
};


#endif // RemoteServerDispatcher_h__