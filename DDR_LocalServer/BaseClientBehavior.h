#ifndef BaseClientBehavior_h__
#define BaseClientBehavior_h__
#include "../../Shared/src/Logic/BaseBehavior.h"
#include "asio.hpp"
#include "../../Shared/src/Utility/Timer.hpp"
class BaseClientBehavior : public DDRFramework::BaseBehavior
{
public:
	BaseClientBehavior();
	~BaseClientBehavior();

	virtual void OnStart(DDRFramework::TcpSocketContainer& container) override;
	virtual void Update(DDRFramework::TcpSocketContainer& container) override;
	virtual void OnStop(DDRFramework::TcpSocketContainer& container) override;

	void ResetHeartBeat();
protected:
	void HeartBeatTimeout(DDRFramework::timer_id id);

	void ThreadEntry();

	DDRFramework::Timer m_Timer;
	DDRFramework::timer_id m_HeartBeatTimerID;
};


#endif // BaseClientBehavior_h__