#include "StreamProxyManager.h"
#include "GlobalManager.h"


StreamProxyManager::StreamProxyManager() : m_StreamProxyConfigLoader("Config/RemoteServer/StreamSrcProxyConfig.xml")
{

	LoadRemoteStreamChannels();
}

StreamProxyManager::~StreamProxyManager()
{
}


void StreamProxyManager::LoadRemoteStreamChannels()
{
	auto config = m_StreamProxyConfigLoader;
	int channelNum = config.RowGetKeyCount();


	for (int i = 0; i < channelNum; i++)
	{
		RemoteStreamChannel rsChannel;

		auto sStreamType = config.GetValue(i, "Type");


		RemoteStreamChannel_StreamType streamType;
		if (sStreamType == "Audio")
		{
			streamType = RemoteStreamChannel_StreamType_Audio;
		}
		else if (sStreamType == "Video")
		{
			streamType = RemoteStreamChannel_StreamType_Video;
		}
		if (sStreamType == "VideoAudio")
		{
			streamType = RemoteStreamChannel_StreamType_VideoAudio;
		}
		rsChannel.set_type(streamType);


		auto bandWidth = config.GetValue<int>(i, "BandWidth");
		rsChannel.set_bandwidth(bandWidth);

		rsChannel.set_url(config.GetValue(i, "Url"));
		rsChannel.set_srcname(config.GetValue(i, "SrcName"));



		StreamProxyChannel channel;
		channel.mIp = config.GetValue(i, "IP");
		channel.mIp = config.GetValue<int>(i, "Port");
		channel.mUsed = 0;

		m_ProxyVec.push_back(channel);

		if (m_ProxyMap.find(channel.mIp) == m_ProxyMap.end())
		{
			m_ProxyMap[channel.mIp] = channel;
		}
	}
}

