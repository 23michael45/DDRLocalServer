#ifndef AudioTcpClient_h__
#define AudioTcpClient_h__


#ifdef QT_PRECOMPILED_HEADER
#else
#include "src/Utility/AudioCodec.h"
#include "src/Network/TcpClientBase.h"
#include "src/Utility/Singleton.h"
#include "src/Utility/Timer.hpp"

using namespace DDRFramework;

class TcpAudioClientSession : public HookTcpClientSession
{
public:
	TcpAudioClientSession(asio::io_context& context);
	~TcpAudioClientSession();


	SHARED_FROM_BASE(TcpAudioClientSession)

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

	SHARED_FROM_BASE(AudioTcpClient)



private:


	DDRFramework::Timer m_Timer;
	DDRFramework::timer_id m_HeartBeatTimerID;
};

#endif
#endif // AudioTcpClient_h__
