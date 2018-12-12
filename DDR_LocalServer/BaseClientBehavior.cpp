#include "BaseClientBehavior.h"
#include "../../Shared/src/Network/TcpSocketContainer.h"
#include "../../Shared/src/Utility/DDRMacro.h"
#include <chrono>
#include <thread>

BaseClientBehavior::BaseClientBehavior()
{
}

BaseClientBehavior::~BaseClientBehavior()
{
	DebugLog("\nBaseClientBehavior Destroy")
}
void BaseClientBehavior::OnStart(DDRFramework::TcpSocketContainer& container)
{
	auto interval = std::chrono::seconds(5);
	m_HeartBeatTimerID = m_Timer.add(interval, std::bind(&BaseClientBehavior::HeartBeatTimeout, this, std::placeholders::_1));
}
void BaseClientBehavior::ThreadEntry()
{

}
void BaseClientBehavior::Update(DDRFramework::TcpSocketContainer& container)
{

}
void BaseClientBehavior::OnStop(DDRFramework::TcpSocketContainer& container)
{

}

void BaseClientBehavior::HeartBeatTimeout(DDRFramework::timer_id id)
{

	DebugLog("\nHeartBeat Timeout----------------------------------------------------------------")
}
void BaseClientBehavior::ResetHeartBeat()
{
	m_Timer.reset(m_HeartBeatTimerID);
	DebugLog("\nResetHeartBeat----------------------------------------------")
	
	
}