#ifndef LocalServerDispatcher_h__
#define LocalServerDispatcher_h__

#include "../../../Shared/src/Network/BaseMessageDispatcher.h"


class BroadcastServerDispatcher : public DDRFramework::BaseMessageDispatcher
{
public:
	BroadcastServerDispatcher();
	~BroadcastServerDispatcher();
};


#endif // LocalServerDispatcher_h__