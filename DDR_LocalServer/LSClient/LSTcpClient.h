#ifndef LSTcpClient_h__
#define LSTcpClient_h__



#include "../../../Shared/src/Network/TcpClientBase.h"
#include "../../../Shared/src/Utility/Singleton.h"
#include "../../../Shared/src/Utility/Timer.hpp"
#include "../../../Shared/src/Utility/DeviceUDID.h"

using namespace DDRFramework;
class LSTcpClient : public TcpClientBase 
{
public:
	LSTcpClient();
	~LSTcpClient();

	void OnConnected(std::shared_ptr<TcpSocketContainer> spContainer) override;
	void OnDisconnect(std::shared_ptr<TcpSocketContainer> spContainer) override;

	virtual std::shared_ptr<TcpClientSessionBase> BindSerializerDispatcher();

	SHARED_FROM_BASE(LSTcpClient)

	void StartHeartBeat();
	void StopHeartBeat();


	void RegisteToRemote();


private:
	void SendHeartBeatOnce(timer_id id);




	DDRFramework::Timer m_Timer;
	DDRFramework::timer_id m_HeartBeatTimerID;
};

class LSBroadcastReceiveTcpClient : public TcpClientBase
{
public:
	LSBroadcastReceiveTcpClient();
	~LSBroadcastReceiveTcpClient();

	void OnConnected(std::shared_ptr<TcpSocketContainer> spContainer) override;

	virtual std::shared_ptr<TcpClientSessionBase> BindSerializerDispatcher();

	SHARED_FROM_BASE(LSBroadcastReceiveTcpClient)

};


#endif // LSTcpClient_h__
