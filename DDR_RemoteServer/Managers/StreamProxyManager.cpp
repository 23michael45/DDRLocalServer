#include "StreamProxyManager.h"
#include "GlobalManager.h"

#include <crossguid/guid.hpp>


StreamProxyManager::StreamProxyManager() : m_StreamProxyConfigLoader("Config/RemoteServer/StreamProxyConfig.xml")
{

	LoadRemoteStreamChannels();
}

StreamProxyManager::~StreamProxyManager()
{
	for (auto pProxy : m_ProxyVec)
	{
		delete pProxy;
	}

	m_ProxyVec.clear();
	m_ProxyMap.clear();
	m_Robot2ProxyMap.clear();

}




RemoteStreamChannel StreamProxyManager::AllocMostIdleProxyUpload(std::string udid, RemoteStreamChannel& channel)
{


	StreamProxy* pproxy = GetMostIdleProxy();
	pproxy->mUploadUsed += channel.uploadbandwidth();




	if (m_Robot2ProxyMap.find(udid) == m_Robot2ProxyMap.end())
	{
		std::vector<StreamProxyChannel> vec;
		m_Robot2ProxyMap[udid] = vec;
	}


	auto& Vec = m_Robot2ProxyMap[udid];


	auto g = xg::newGuid();
	std::string guid = g.str();


	StreamProxyChannel proxychannel;
	proxychannel.mChannel = channel;
	proxychannel.mProxy = pproxy;
	proxychannel.mStreamUUID = guid;


	Vec.push_back(proxychannel);

	channel.set_url(GetRtspFullAddr(proxychannel));

	return channel;
}

void StreamProxyManager::ReleaseProxyUpload(std::string udid)
{
	auto vec = m_Robot2ProxyMap[udid];
	for (auto proxychannel : vec)
	{
		proxychannel.mProxy->mUploadUsed -= proxychannel.mChannel.uploadbandwidth();
	}
	m_Robot2ProxyMap.erase(udid);
}


StreamProxyManager::StreamProxy* StreamProxyManager::AllocMostIdleProxyDownload(RemoteStreamChannel& channel)
{
	StreamProxy* pproxy = GetMostIdleProxy();
	pproxy->mDownloadUsed += channel.uploadbandwidth();
	return pproxy;
}

void StreamProxyManager::ReleaseProxyDownload(std::string username)
{

}

bool StreamProxyManager::compare(StreamProxy* a, StreamProxy* b)
{
	return a->mUploadUsed < b->mUploadUsed; //升序排列，如果改为return a>b，则为降序
}

StreamProxyManager::StreamProxy* StreamProxyManager::GetMostIdleProxy()
{
	if (m_ProxyVec.size() > 0)
	{
		std::sort(m_ProxyVec.begin(), m_ProxyVec.end(), std::bind(&StreamProxyManager::compare, this, std::placeholders::_1, std::placeholders::_2));
		auto pleastOne = m_ProxyVec[0];
		return pleastOne;
	}

	return nullptr;
}

void StreamProxyManager::LoadRemoteStreamChannels()
{
	auto config = m_StreamProxyConfigLoader;
	int channelNum = config.RowGetKeyCount();


	for (int i = 0; i < channelNum; i++)
	{
		StreamProxy sproxy;

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


		StreamProxy* pproxy = new StreamProxy();


		auto uploadbandWidth = config.GetValue<int>(i, "UploadBandWidth");
		pproxy->mUploadBandWidth = uploadbandWidth;


		auto downloadbandWidth = config.GetValue<int>(i, "DownloadBandWidth");
		pproxy->mDonwloadBandWidth = downloadbandWidth;

		pproxy->mUrl = config.GetValue(i, "Url");
		pproxy->mName = config.GetValue(i, "SrcName");



		pproxy->mIp = config.GetValue(i, "IP");
		pproxy->mPort = config.GetValue<int>(i, "Port");
		pproxy->mUploadUsed = 0;
		pproxy->mDownloadUsed = 0;

		m_ProxyVec.push_back(pproxy);

		if (m_ProxyMap.find(pproxy->mIp) == m_ProxyMap.end())
		{
			m_ProxyMap[pproxy->mIp] = pproxy;
		}
	}
}

