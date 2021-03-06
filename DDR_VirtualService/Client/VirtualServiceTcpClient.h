#ifndef VirtualServiceTcpClient_h__
#define VirtualServiceTcpClient_h__

#include "src/Network/TcpClientBase.h"
#include "src/Utility/Singleton.h"
#include "src/Utility/Timer.hpp"

using namespace DDRFramework;
class VirtualServiceTcpClient : public TcpClientBase 
{
public:
	VirtualServiceTcpClient();
	~VirtualServiceTcpClient();

	void OnConnected(std::shared_ptr<TcpSocketContainer> spContainer) override;
	void OnDisconnect(std::shared_ptr<TcpSocketContainer> spContainer) override;

	virtual std::shared_ptr<TcpClientSessionBase> BindSerializerDispatcher();

	SHARED_FROM_BASE(VirtualServiceTcpClient)


	void StartHeartBeat();
	void StopHeartBeat();

	void RequestStreamInfo();

private:
	void SendHeartBeatOnce(timer_id id);




	DDRFramework::Timer m_Timer;
	DDRFramework::timer_id m_HeartBeatTimerID;
};


#endif // VirtualServiceTcpClient_h__
