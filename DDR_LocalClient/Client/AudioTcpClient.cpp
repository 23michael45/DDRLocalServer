#include "AudioTcpClient.h"
#include "../../../Shared/src/Network/TcpSocketContainer.h"
#include "../../../Shared/src/Network/MessageSerializer.h"
#include "../../../Shared/src/Network/BaseMessageDispatcher.h"
#include "LocalClientDispatcher.h"
#include "LocalClientUdpDispatcher.h"


#include "../../../Shared/proto/BaseCmd.pb.h"
#include "GlobalManager.h"
using namespace DDRCommProto;

AudioTcpClient::AudioTcpClient()
{
}


AudioTcpClient::~AudioTcpClient()
{
}

std::shared_ptr<TcpClientSessionBase> AudioTcpClient::BindSerializerDispatcher()
{
	BIND_IOCONTEXT_SERIALIZER_DISPATCHER(m_IOContext, TcpClientSessionBase, MessageSerializer, BaseMessageDispatcher,BaseHeadRuleRouter)
		return spTcpClientSessionBase;
}
void AudioTcpClient::OnConnected(TcpSocketContainer& container)
{

	DebugLog("\nOnConnectSuccess! LocalTcpClient");

	m_AudioCodec.Init(2, 48000);
	m_AudioCodec.StartRecord();
	//m_AudioCodec.StartPlay();

}
void AudioTcpClient::OnDisconnect(TcpSocketContainer& container)
{
	TcpClientBase::OnDisconnect(container);

	m_AudioCodec.StopRecord();
	//m_AudioCodec.StopPlay();
}
