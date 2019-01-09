#ifndef LSClientDispatcher_h__
#define LSClientDispatcher_h__



#include "../../../Shared/src/Network/BaseMessageDispatcher.h"


class LSClientDispatcher : public DDRFramework::BaseMessageDispatcher
{
public:
	LSClientDispatcher();
	~LSClientDispatcher();
};


#endif // LSClientDispatcher_h__