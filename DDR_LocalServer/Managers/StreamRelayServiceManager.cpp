#include "StreamRelayServiceManager.h"
#include "GlobalManager.h"


StreamRelayServiceManager::StreamRelayServiceManager() : m_StreamSrcProxyConfig("Config/LocalServer/StreamSrcProxyConfig.xml")
{


}

StreamRelayServiceManager::~StreamRelayServiceManager()
{
}


std::vector<AVChannelConfig> StreamRelayServiceManager::GetAVChannelsConfig()
{
	auto config = m_StreamSrcProxyConfig;
	int channelNum = config.RowGetKeyCount();

	std::vector<AVChannelConfig> channelvec;
	for (int i = 0; i < channelNum; i++)
	{
		AVChannelConfig channel;

		auto sStreamType = config.GetValue(i, "StreamType");


		ChannelStreamType streamType;
		if (sStreamType == "A")
		{
			streamType = ChannelStreamType::Audio;
		}
		else if (sStreamType == "V")
		{
			streamType = ChannelStreamType::Video;
		}
		if (sStreamType == "AV")
		{
			streamType = ChannelStreamType::VideoAudio;
		}
		channel.set_streamtype(streamType);


		auto sNetworkType = config.GetValue(i, "NetworkType");
		ChannelNetworkType networkType;
		if (sNetworkType == "Local")
		{
			networkType = ChannelNetworkType::Local;
		}
		else if (sNetworkType == "Remote")
		{
			networkType = ChannelNetworkType::Remote;
		}
		channel.set_networktype(networkType);

		channel.set_rate(config.GetValue<int>(i, "Rate"));
		channel.set_src(config.GetValue(i, "SrcAddress"));
		channel.set_dst(config.GetValue(i, "RemoteAddress"));


		channelvec.push_back(channel);
	}
	return channelvec;
}


int StreamRelayServiceManager::GetServerTcpPort()
{
	return GlobalManager::Instance()->GetLocalServerConfig().GetValue<int>("StreamRelayTcpPort");

}