#ifndef StreamRelayServiceManager_h__
#define StreamRelayServiceManager_h__

#include "../../../Shared/src/Utility/XmlLoader.h"
#include "../../../Shared/src/Utility/Singleton.h"

#include "../../../Shared/src/Network/TcpServerBase.h"
#include "../../../Shared/proto/BaseCmd.pb.h"

using namespace DDRCommProto;
using namespace DDRFramework;
class StreamRelayServiceManager : public CSingleton<StreamRelayServiceManager>
{
public:
	StreamRelayServiceManager();
	~StreamRelayServiceManager();

	XmlLoader& GetStreamSrcProxyConfig()
	{
		return m_StreamSrcProxyConfig;
	}

	std::vector<AVChannelConfig> GetAVChannels();

	
	std::shared_ptr<TcpSessionBase> GetServerSession();
	int GetServerTcpPort();
private:


	XmlLoader m_StreamSrcProxyConfig;
};



#endif // StreamRelayServiceManager_h__
