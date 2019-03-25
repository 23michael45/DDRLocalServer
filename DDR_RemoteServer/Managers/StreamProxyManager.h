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
public:
	struct StreamProxy
	{
	public:
		std::string mUrl;
		int mUploadBandWidth = 0;
		int mDonwloadBandWidth = 0;

		int mUploadUsed = 0;
		int mDownloadUsed = 0;
		std::string mIp;
		int mPort;

		std::string mName;
	};

	struct StreamProxyChannel
	{
		RemoteStreamChannel mChannel;
		StreamProxy* mProxy;
		std::string mStreamUUID;
	};


	RemoteStreamChannel AllocMostIdleProxyUpload(std::string udid,RemoteStreamChannel& channel);
	void ReleaseProxyUpload(std::string udid);

	StreamProxyManager::StreamProxy* AllocMostIdleProxyDownload(RemoteStreamChannel& channel);
	void ReleaseProxyDownload(std::string username);


	std::vector<RemoteStreamChannel> GetRobotUploadAddr(std::string udid)
	{
		std::vector< RemoteStreamChannel> vec;

		if (m_Robot2ProxyMap.find(udid) != m_Robot2ProxyMap.end())
		{
			for (auto c : m_Robot2ProxyMap[udid])
			{
				c.mChannel.set_url(GetRtspFullAddr(c));
				vec.push_back(c.mChannel);
			}
		}
		return vec;
	}

private:

	bool compare(StreamProxy* a, StreamProxy* b);

	StreamProxy* GetMostIdleProxy();

	std::string GetRtspFullAddr(StreamProxyChannel channel)
	{
		return channel.mProxy->mUrl + "/" + channel.mStreamUUID;
	}



public:
	StreamProxyManager();
	~StreamProxyManager();


	void LoadRemoteStreamChannels();


private:


	XmlLoader m_StreamProxyConfigLoader;
	std::vector<RemoteStreamChannel> m_StreamProxyConfig;


	std::vector<StreamProxy*> m_ProxyVec;
	std::map<std::string, StreamProxy*> m_ProxyMap;



	std::map<std::string, std::vector<StreamProxyChannel>> m_Robot2ProxyMap;


};



#endif // StreamProxyConfigManager_h__