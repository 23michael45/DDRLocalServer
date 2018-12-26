#include "AudioTcpClient.h"
#include "../../../Shared/src/Network/TcpSocketContainer.h"
#include "../../../Shared/src/Network/MessageSerializer.h"
#include "../../../Shared/src/Network/BaseMessageDispatcher.h"
#include "LocalClientDispatcher.h"
#include "LocalClientUdpDispatcher.h"


#include "../../../Shared/proto/BaseCmd.pb.h"
#include "GlobalManager.h"
using namespace DDRCommProto;



AudioTcpClient* p_gAudioClient;

void on_recv_frames(mal_device* pDevice, mal_uint32 frameCount, const void* pSamples)
{
	mal_uint32 sampleCount = frameCount * pDevice->channels;

	/*mal_uint32 newCapturedSampleCount = capturedSampleCount + sampleCount;
	mal_int16* pNewCapturedSamples = (mal_int16*)realloc(pCapturedSamples, newCapturedSampleCount * sizeof(mal_int16));
	if (pNewCapturedSamples == NULL) {
		return;
	}

	memcpy(pNewCapturedSamples + capturedSampleCount, pSamples, sampleCount * sizeof(mal_int16));

	pCapturedSamples = pNewCapturedSamples;
	capturedSampleCount = newCapturedSampleCount;*/


	std::shared_ptr<asio::streambuf> buf = std::make_shared<asio::streambuf>();

	std::ostream oshold(buf.get());
	oshold.write((const char*)pSamples, sampleCount * sizeof(mal_int16));
	oshold.flush();

	if (p_gAudioClient)
	{
		p_gAudioClient->Send(buf);
	}

}

mal_uint32 on_send_frames(mal_device* pDevice, mal_uint32 frameCount, void* pSamples)
{
	mal_uint32 samplesToRead = frameCount * pDevice->channels;
	/*if (samplesToRead > capturedSampleCount - playbackSample) {
		samplesToRead = capturedSampleCount - playbackSample;
	}

	if (samplesToRead == 0) {
		return 0;
	}

	memcpy(pSamples, pCapturedSamples + playbackSample, samplesToRead * sizeof(mal_int16));
	playbackSample += samplesToRead;*/

	return samplesToRead / pDevice->channels;
}



TcpAudioClientSession::TcpAudioClientSession(asio::io_context& context):TcpClientSessionBase(context)
{
	SetRealtime(true);
}
TcpAudioClientSession::~TcpAudioClientSession()
{
	DebugLog("\nDestroy TcpAudioClientSession");
}


AudioTcpClient::AudioTcpClient()
{
	p_gAudioClient = this;
}


AudioTcpClient::~AudioTcpClient()
{
}

std::shared_ptr<TcpClientSessionBase> AudioTcpClient::BindSerializerDispatcher()
{
	BIND_IOCONTEXT_SERIALIZER_DISPATCHER(m_IOContext, TcpAudioClientSession, MessageSerializer, BaseMessageDispatcher,BaseHeadRuleRouter)
		return spTcpAudioClientSession;
}
void AudioTcpClient::OnConnected(std::shared_ptr<TcpSocketContainer> spContainer)
{

	DebugLog("\nOnConnectSuccess! LocalTcpClient");

	m_AudioCodec.Init(1, 16000, on_recv_frames, on_send_frames);
	m_AudioCodec.StartRecord();
	//m_AudioCodec.StartPlay();

}
void AudioTcpClient::OnDisconnect(std::shared_ptr<TcpSocketContainer> spContainer)
{
	TcpClientBase::OnDisconnect(spContainer);

	m_AudioCodec.StopRecord();
	//m_AudioCodec.StopPlay();
}
