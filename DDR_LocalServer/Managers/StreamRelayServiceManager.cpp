#include "StreamRelayServiceManager.h"
#include "GlobalManager.h"


StreamRelayServiceManager::StreamRelayServiceManager() : m_StreamSrcProxyConfig("Config/LocalServer/StreamSrcProxyConfig.xml")
{


}

StreamRelayServiceManager::~StreamRelayServiceManager()
{
}


std::vector<AVChannel> StreamRelayServiceManager::GetAVChannels()
{
	auto config = m_StreamSrcProxyConfig;
	int channelNum = config.RowGetKeyCount();

	std::vector<AVChannel> channelvec;
	for (int i = 0; i < channelNum; i++)
	{
		AVChannel channel;

		auto sType = config.GetValue(i, "Type");
		ChannelStreamType streamType;
		if (sType == "A")
		{
			streamType = ChannelStreamType::Audio;
		}
		else if (sType == "V")
		{
			streamType = ChannelStreamType::Video;
		}
		if (sType == "AV")
		{
			streamType = ChannelStreamType::VideoAudio;
		}

		channel.set_streamtype(streamType);

		channel.set_src(config.GetValue(i, "SrcAddress"));
		channel.set_dst(config.GetValue(i, "RemoteAddress"));


		channelvec.push_back(channel);
	}
	return channelvec;
}
std::shared_ptr<TcpSessionBase> StreamRelayServiceManager::GetServerSession()
{
	auto sessionMap = GlobalManager::Instance()->GetTcpServer()->GetTcpSocketContainerMap();
	for (auto session : sessionMap)
	{
		if (session.second->GetLoginInfo().type() == eCltType::eLSMStreamRelay)
		{
			return session.second;
		}
	}
	return nullptr;
}


int StreamRelayServiceManager::GetServerTcpPort()
{
	return GlobalManager::Instance()->GetLocalServerConfig().GetValue<int>("StreamRelayTcpPort");

}