#include "../Behaviors/BaseClientBehavior.h"
#include "../../../Shared/src/Network/TcpSocketContainer.h"
#include "../../../Shared/src/Utility/DDRMacro.h"
#include <chrono>
#include <thread>

#include "../../../Shared/src/Utility/Logger.h"
BaseClientBehavior::BaseClientBehavior()
{
}

BaseClientBehavior::~BaseClientBehavior()
{
	m_Timer.remove(m_HeartBeatTimerID);
	DebugLog("\nBaseClientBehavior Destroy")
}
void BaseClientBehavior::OnStart(std::shared_ptr<DDRFramework::TcpSocketContainer> spContainer)
{
	DebugLog("\nBaseClientBehavior OnStart----------------------------------------------------------------")
	auto interval = std::chrono::seconds(15);
	m_HeartBeatTimerID = m_Timer.add(interval, std::bind(&BaseClientBehavior::HeartBeatTimeout, this, spContainer));
}
void BaseClientBehavior::Update(std::shared_ptr<DDRFramework::TcpSocketContainer> spContainer)
{

}
void BaseClientBehavior::OnStop(std::shared_ptr<DDRFramework::TcpSocketContainer> spContainer)
{
	DebugLog("\nBaseClientBehavior OnStop----------------------------------------------------------------")
	m_Timer.remove(m_HeartBeatTimerID);
}

void BaseClientBehavior::HeartBeatTimeout(std::shared_ptr<DDRFramework::TcpSocketContainer> spContainer)
{
	spContainer->Stop();
	m_Timer.remove(m_HeartBeatTimerID);
	DebugLog("\nHeartBeat Timeout----------------------------------------------------------------")


}
void BaseClientBehavior::ResetHeartBeat(std::shared_ptr<DDRFramework::TcpSocketContainer> spContainer)
{
	m_Timer.reset(m_HeartBeatTimerID);
	//DebugLog("\nResetHeartBeat----------------------------------------------")
	
	
}