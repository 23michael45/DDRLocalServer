#ifndef LSTcpClient_h__
#define LSTcpClient_h__



#include "../../../Shared/src/Network/TcpClientBase.h"
#include "../../../Shared/src/Utility/Singleton.h"
#include "../../../Shared/src/Utility/Timer.hpp"

using namespace DDRFramework;
class LSClient : public TcpClientBase 
{
public:
	LSClient();
	~LSClient();

	void OnConnected(std::shared_ptr<TcpSocketContainer> spContainer) override;
	void OnDisconnect(std::shared_ptr<TcpSocketContainer> spContainer) override;

	virtual std::shared_ptr<TcpClientSessionBase> BindSerializerDispatcher();

	auto shared_from_base() {
		return std::dynamic_pointer_cast<LSClient>(shared_from_this());
	}


	void StartHeartBeat();
	void StopHeartBeat();



private:
	void SendHeartBeatOnce(timer_id id);




	DDRFramework::Timer m_Timer;
	DDRFramework::timer_id m_HeartBeatTimerID;
};

#endif // LSTcpClient_h__
