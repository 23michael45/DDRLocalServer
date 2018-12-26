#ifndef AudioTcpClient_h__
#define AudioTcpClient_h__


#ifdef QT_PRECOMPILED_HEADER
#else
#include "../../../Shared/src/Utility/AudioCodec.h"
#include "../../../Shared/src/Network/TcpClientBase.h"
#include "../../../Shared/src/Utility/Singleton.h"
#include "../../../Shared/src/Utility/Timer.hpp"

using namespace DDRFramework;

class TcpAudioClientSession : public TcpClientSessionBase
{
public:
	TcpAudioClientSession(asio::io_context& context);
	~TcpAudioClientSession();
};


class AudioTcpClient : public TcpClientBase 
{
public:
	AudioTcpClient();
	~AudioTcpClient();

	void OnConnected(std::shared_ptr<TcpSocketContainer> spContainer) override;
	void OnDisconnect(std::shared_ptr<TcpSocketContainer> psContainer) override;

	virtual std::shared_ptr<TcpClientSessionBase> BindSerializerDispatcher();

	auto shared_from_base() {
		return std::dynamic_pointer_cast<AudioTcpClient>(shared_from_this());
	}



private:


	AudioCodec m_AudioCodec;

	DDRFramework::Timer m_Timer;
	DDRFramework::timer_id m_HeartBeatTimerID;
};

#endif
#endif // AudioTcpClient_h__
