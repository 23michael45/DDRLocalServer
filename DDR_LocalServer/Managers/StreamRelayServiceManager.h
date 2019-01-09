#ifndef StreamRelayServiceManager_h__
#define StreamRelayServiceManager_h__

#include "../../../Shared/src/Utility/XmlLoader.h"
#include "../../../Shared/src/Utility/Singleton.h"

#include "../../../Shared/src/Network/TcpServerBase.h"
#include "../../../Shared/proto/BaseCmd.pb.h"
#include "eventpp/eventdispatcher.h"
#include "../Servers/LocalTcpServer.h"

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

	std::vector<AVChannelConfig> GetAVChannelsConfig();

	
	std::shared_ptr<LocalServerTcpSession> GetServerSession();
	void Send(std::shared_ptr<google::protobuf::Message> spmsg);
	void Send(std::shared_ptr<CommonHeader> spHeader, asio::streambuf& buf, int bodylen);
	
	int GetServerTcpPort();

private:


	XmlLoader m_StreamSrcProxyConfig;


};



#endif // StreamRelayServiceManager_h__
