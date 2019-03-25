#include "StreamRelayServiceManager.h"
#include "GlobalManager.h"


StreamRelayServiceManager::StreamRelayServiceManager() : m_StreamSrcProxyConfigLoader("Config/LocalServer/StreamSrcProxyConfig.xml")
{

	InitAVChannels();
	InitChannelsToUpload();
}

StreamRelayServiceManager::~StreamRelayServiceManager()
{
}

void StreamRelayServiceManager::InitAVChannels()
{
	auto config = m_StreamSrcProxyConfigLoader;
	int channelNum = config.RowGetKeyCount();


	for (int i = 0; i < channelNum; i++)
	{
		LocalStreamSrc channel;

		auto sStreamType = config.GetValue(i, "StreamType");


		LocalStreamSrc::EStreamType  streamType;
		if (sStreamType == "A")
		{
			streamType = LocalStreamSrc::EStreamType::Audio;
		}
		else if (sStreamType == "V")
		{
			streamType = LocalStreamSrc::EStreamType::Video;
		}
		if (sStreamType == "AV")
		{
			streamType = LocalStreamSrc::EStreamType::VideoAudio;
		}
		channel.mType = (streamType);



		channel.mBandWidth = (config.GetValue<int>(i, "BandWidth"));
		channel.mSrc = (config.GetValue(i, "SrcAddress"));
		channel.mSrcName = (config.GetValue(i, "SrcName"));


		m_LocalStreamConfig.push_back(channel);
	}
}


void StreamRelayServiceManager::InitChannelsToUpload()
{
	for (auto channel : m_LocalStreamConfig)
	{

		RemoteStreamChannel c;
		c.set_url("");//do not need fill url

		if (channel.mType == LocalStreamSrc::EStreamType::Video)
		{

			c.set_type(RemoteStreamChannel_StreamType_Video);
		}
		else if (channel.mType == LocalStreamSrc::EStreamType::Audio)
		{

			c.set_type(RemoteStreamChannel_StreamType_Audio);
		}
		else if (channel.mType == LocalStreamSrc::EStreamType::VideoAudio)
		{

			c.set_type(RemoteStreamChannel_StreamType_VideoAudio);
		}


		c.set_uploadbandwidth(channel.mBandWidth);
		c.set_downloadbandwidth(0);//do not need fill downloadbandwidth
		c.set_srcname(channel.mSrcName);

		m_ChannelsToUploadConfig.push_back(c);

	}
}

int StreamRelayServiceManager::GetServerTcpPort()
{
	return GlobalManager::Instance()->GetLocalServerConfig().GetValue<int>("StreamRelayTcpPort");

}

void StreamRelayServiceManager::UpdateUploadServer(rspRtspStreamUploadAddr* prsp)
{
	for (int i = 0; i < prsp->channels_size(); i++)
	{
		auto& channel = prsp->channels(i);
		m_ChannelsToUploadOnRemoteServer.push_back(channel);
	}
}
