#ifndef LocalTcpClient_h__
#define LocalTcpClient_h__

#include "../../../Shared/src/Network/TcpClientBase.h"
#include "../../../Shared/src/Utility/Singleton.h"
#include "../../../Shared/src/Utility/Timer.hpp"

using namespace DDRFramework;
class LocalTcpClient : public TcpClientBase 
{
public:
	LocalTcpClient();
	~LocalTcpClient();

	void OnConnected(TcpSocketContainer& container) override;
	void OnDisconnect(TcpSocketContainer& container) override;

	virtual std::shared_ptr<TcpClientSessionBase> BindSerializerDispatcher();

	auto shared_from_base() {
		return std::dynamic_pointer_cast<LocalTcpClient>(shared_from_this());
	}


	void StartHeartBeat();
	void StopHeartBeat();



private:
	void SendHeartBeatOnce(timer_id id);




	DDRFramework::Timer m_Timer;
	DDRFramework::timer_id m_HeartBeatTimerID;
};


#endif // LocalTcpClient_h__