#ifndef GlobalManager_h__
#define GlobalManager_h__

#include "../../../Shared/src/Network/UdpSocketBase.h"
#include "../../../Shared/src/Network/TcpClientBase.h"
#include "../../../Shared/src/Utility/Singleton.h"
#include "LocalTcpClient.h"
#include "AudioTcpClient.h"

#include "../../../Shared/src/Utility/LocalizationLoader.h"

using namespace DDRFramework;
class GlobalManager : public DDRFramework::CSingleton<GlobalManager>
{
public:
	GlobalManager();
	~GlobalManager();

public:

	void StartUdp();
	void ReleaseUdp();
	bool IsUdpWorking();

	void StartAudioClient(std::string ip, int port);
	void StopAudioClient();


	void StartTcpClient();
	void ReleaseTcp();
	bool IsTcpWorking();


	LocalizationLoader& GetLocalizationConfig()
	{
		return m_LocalizationConfig;
	}
	
	std::shared_ptr<LocalTcpClient> GetTcpClient();
	std::shared_ptr<UdpSocketBase> GetUdpClient();
	std::shared_ptr<AudioTcpClient> GetAudioTcpClient();

private:

	void OnUdpDisconnect(UdpSocketBase& container);

	std::shared_ptr<LocalTcpClient> m_spTcpClient;
	std::shared_ptr<UdpSocketBase> m_spUdpClient;
	std::shared_ptr<AudioTcpClient> m_spAudioTcpClient;


	LocalizationLoader m_LocalizationConfig;

};

#endif // GlobalManager_h__
