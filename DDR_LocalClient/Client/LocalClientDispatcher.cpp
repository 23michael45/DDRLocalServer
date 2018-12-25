#include "LocalClientDispatcher.h"
#include "../../../Shared/proto/BaseCmd.pb.h"

#ifdef QT_PRECOMPILED_HEADER
#include "LoginProcessor.h"
#include "../../Dadaoii_Proto/Proto/AudioStreamClientProcessor.h"
#else
#include "LoginProcessor.h"
#include "StreamAddrProcessor.h"
#include "AlarmProcessor.h"
#include "FileAddressProcessor.h"
#endif
using namespace DDRCommProto;
using namespace DDRFramework;

LocalClientDispatcher::LocalClientDispatcher()
{

#ifdef QT_PRECOMPILED_HEADER
		RegisterProcessor(rsp, Login)
        RegisterProcessor(rsp, StreamAddr)
#else
        RegisterProcessor(rsp, Login)
        RegisterProcessor(rsp, StreamAddr)
        RegisterProcessor(notify, Alarm)
        RegisterProcessor(rsp, FileAddress)
#endif
}


LocalClientDispatcher::~LocalClientDispatcher()
{
}
