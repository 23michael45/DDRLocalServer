#ifndef StreamProxyConfigManager_h__
#define StreamProxyConfigManager_h__


#include "../../../Shared/src/Utility/XmlLoader.h"
#include "../../../Shared/src/Utility/Singleton.h"

#include "../../../Shared/src/Network/TcpServerBase.h"
#include "../../../Shared/proto/RemoteCmd.pb.h"
#include "eventpp/eventdispatcher.h"

using namespace DDRCommProto;
using namespace DDRFramework;
class StreamProxyManager : public CSingleton<StreamProxyManager>
{
	struct StreamProxyChannel
	{
		RemoteStreamChannel mChannel;
		int mUsed;
		std::string mIp;
		int mPort;
	};


	bool compare(StreamProxyChannel a, StreamProxyChannel b)
	{
		return a.mUsed < b.mUsed; //升序排列，如果改为return a>b，则为降序

	}

	StreamProxyChannel GetMostIdleProxy()
	{
		if (m_ProxyVec.size() > 0)
		{
			std::sort(m_ProxyVec.begin(), m_ProxyVec.end(), std::bind(&StreamProxyManager::compare,this,std::placeholders::_1,std::placeholders::_2));
			auto leastOne = m_ProxyVec[0];
			return leastOne;
		}

	}



public:
	StreamProxyManager();
	~StreamProxyManager();


	void LoadRemoteStreamChannels();


private:


	XmlLoader m_StreamProxyConfigLoader;
	std::vector<RemoteStreamChannel> m_StreamProxyConfig;


	std::vector<StreamProxyChannel> m_ProxyVec;
	std::map<std::string, StreamProxyChannel> m_ProxyMap;


};



#endif // StreamProxyConfigManager_h__