#ifndef AudioTcpClient_h__
#define AudioTcpClient_h__


#ifdef QT_PRECOMPILED_HEADER
#else
#include "../../../Shared/src/Utility/AudioCodec.h"
#include "../../../Shared/src/Network/TcpClientBase.h"
#include "../../../Shared/src/Utility/Singleton.h"
#include "../../../Shared/src/Utility/Timer.hpp"

using namespace DDRFramework;

class TcpAudioClientSession : public HookTcpClientSession
{
public:
	TcpAudioClientSession(asio::io_context& context);
	~TcpAudioClientSession();

	auto shared_from_base() {
		return std::static_pointer_cast<TcpAudioClientSession>(shared_from_this());
	}
	virtual void OnStart() override;
	virtual void OnStop() override;

	virtual void OnHookReceive(asio::streambuf& buf) override ;

protected:

	AudioCodec m_AudioCodec;

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


	DDRFramework::Timer m_Timer;
	DDRFramework::timer_id m_HeartBeatTimerID;
};

#endif
#endif // AudioTcpClient_h__
