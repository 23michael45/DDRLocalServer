#include "AudioTcpClient.h"
#include "../../../Shared/src/Network/TcpSocketContainer.h"
#include "../../../Shared/src/Network/MessageSerializer.h"
#include "../../../Shared/src/Network/BaseMessageDispatcher.h"
#include "LocalClientDispatcher.h"
#include "LocalClientUdpDispatcher.h"


#include "../../../Shared/proto/BaseCmd.pb.h"
#include "GlobalManager.h"
using namespace DDRCommProto;




void TcpAudioClientSession::on_recv_frames(mal_device* pDevice, mal_uint32 frameCount, const void* pSamples)
{
	mal_uint32 sampleCount = frameCount * pDevice->channels;

	std::shared_ptr<asio::streambuf> buf = std::make_shared<asio::streambuf>();

	std::ostream oshold(buf.get());
	oshold.write((const char*)pSamples, sampleCount * sizeof(mal_int16));
	oshold.flush();

	Send(buf);


}

mal_uint32 TcpAudioClientSession::on_send_frames(mal_device* pDevice, mal_uint32 frameCount, void* pSamples)
{
	std::lock_guard<std::mutex> lock(m_AudioRecvMutex);
	mal_uint32 samplesToRead = frameCount * pDevice->channels;

	asio::streambuf& buf = m_AudioRecvBuf;

	size_t len = buf.size();
	if (len < samplesToRead * sizeof(mal_int16))
	{

		memcpy(pSamples, buf.data().data(), len);
		buf.consume(len);

		return len / sizeof(mal_int16) / pDevice->channels;
	}
	else
	{

		memcpy(pSamples, buf.data().data(), samplesToRead * sizeof(mal_int16));
		buf.consume(samplesToRead * sizeof(mal_int16));
		return samplesToRead / pDevice->channels;
	}

}

void TcpAudioClientSession::OnHookReceive(asio::streambuf& buf)
{
	std::lock_guard<std::mutex> lock(m_AudioRecvMutex);
	std::ostream oshold(&m_AudioRecvBuf);
	oshold.write((const char*)buf.data().data(), buf.size());
	oshold.flush();
}
TcpAudioClientSession::TcpAudioClientSession(asio::io_context& context): HookTcpClientSession(context)
{

}
TcpAudioClientSession::~TcpAudioClientSession()
{
	DebugLog("Destroy TcpAudioClientSession");
}

//
//void Test()
//{
//
//
//
//	std::string root = "D:/DevelopProj/Dadao/DDRFramework/www/wav/";
//
//	int totallen = 6400 * 19;
//	char* psrc = new char[totallen];
//	char* p = psrc;
//
//	for (int i = 0; i < 19; i++)
//	{
//		std::string fn = root + std::to_string(i) + ".txt";
//
//		std::ifstream file(fn);
//		std::string   line;
//
//		while (std::getline(file, line))
//		{
//			std::stringstream   linestream(line);
//			std::string         data;
//			char                val;
//
//			// If you have truly tab delimited data use getline() with third parameter.
//			// If your data is just white space separated data
//			// then the operator >> will do (it reads a space separated word into a string).
//			std::getline(linestream, data, '\t');  // read up-to the first tab (discard tab).
//
//			// Read the integers using the operator >>
//			val = atoi(data.c_str());
//
//			*p = val;
//			p++;
//		}
//
//	}
//	Send(psrc, totallen);
//}

void TcpAudioClientSession::OnStart()
{
	//do not use shared_from_base ,member don't give shared_ptr otherwisse it wont destruct correctly
	if (m_AudioCodec.Init())
	{
		m_AudioCodec.StartDeviceRecord();
		m_AudioCodec.StartDevicePlay();

	}

}
void TcpAudioClientSession::OnStop()
{

	m_AudioCodec.StopDeviceRecord();
	m_AudioCodec.Deinit();
	m_AudioCodec.StopDevicePlay();
}


AudioTcpClient::AudioTcpClient()
{
}


AudioTcpClient::~AudioTcpClient()
{
	DebugLog("Destroy AudioTcpClient");
}

std::shared_ptr<TcpClientSessionBase> AudioTcpClient::BindSerializerDispatcher()
{
	BIND_IOCONTEXT_SERIALIZER_DISPATCHER(m_IOContext, TcpAudioClientSession, MessageSerializer, BaseMessageDispatcher,BaseHeadRuleRouter)
		return spTcpAudioClientSession;
}
void AudioTcpClient::OnConnected(std::shared_ptr<TcpSocketContainer> spContainer)
{
	DebugLog("OnConnectSuccess! AudioTcpClient");

}
void AudioTcpClient::OnDisconnect(std::shared_ptr<TcpSocketContainer> spContainer)
{
	TcpClientBase::OnDisconnect(spContainer);
	GlobalManager::Instance()->StopAudioClient();

}
