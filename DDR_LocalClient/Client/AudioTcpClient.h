#ifndef AudioTcpClient_h__
#define AudioTcpClient_h__



#include "../../../Shared/src/Utility/AudioCodec.h"
#include "../../../Shared/src/Network/TcpClientBase.h"
#include "../../../Shared/src/Utility/Singleton.h"
#include "../../../Shared/src/Utility/Timer.hpp"

using namespace DDRFramework;
class AudioTcpClient : public TcpClientBase 
{
public:
	AudioTcpClient();
	~AudioTcpClient();

	void OnConnected(TcpSocketContainer& container) override;
	void OnDisconnect(TcpSocketContainer& container) override;

	virtual std::shared_ptr<TcpClientSessionBase> BindSerializerDispatcher();

	auto shared_from_base() {
		return std::dynamic_pointer_cast<AudioTcpClient>(shared_from_this());
	}



private:


	AudioCodec m_AudioCodec;

	DDRFramework::Timer m_Timer;
	DDRFramework::timer_id m_HeartBeatTimerID;
};

#endif // AudioTcpClient_h__
