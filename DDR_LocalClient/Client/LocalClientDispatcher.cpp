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
#include "RemoteServerListProcessor.h"
#include "RemoteLoginProcessor.h"
#endif
using namespace DDRCommProto;
using namespace DDRFramework;

LocalClientDispatcher::LocalClientDispatcher()
{
        RegisterProcessor(rsp, Login)
        RegisterProcessor(rsp, StreamAddr)
        RegisterProcessor(notify, Alarm)
			RegisterProcessor(rsp, FileAddress)
			RegisterProcessor(rsp, RemoteLogin)

}


LocalClientDispatcher::~LocalClientDispatcher()
{
}

LSBroadcastReceiveClientDispatcher::LSBroadcastReceiveClientDispatcher()
{
	RegisterProcessor(rsp, RemoteServerList)

}


LSBroadcastReceiveClientDispatcher::~LSBroadcastReceiveClientDispatcher()
{
}