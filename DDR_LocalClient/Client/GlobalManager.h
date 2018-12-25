#ifndef GlobalManager_h__
#define GlobalManager_h__

#ifdef QT_PRECOMPILED_HEADER
    #include "stable.h"
#else
    #include "AudioTcpClient.h"
#endif

#include "../../../Shared/src/Network/UdpSocketBase.h"
#include "../../../Shared/src/Network/TcpClientBase.h"
#include "../../../Shared/src/Utility/Singleton.h"
#include "LocalTcpClient.h"
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
#ifdef QT_PRECOMPILED_HEADER
#else
    std::shared_ptr<AudioTcpClient> GetAudioTcpClient();
#endif

private:

	void OnUdpDisconnect(UdpSocketBase& container);

	std::shared_ptr<LocalTcpClient> m_spTcpClient;
	std::shared_ptr<UdpSocketBase> m_spUdpClient;
#ifdef QT_PRECOMPILED_HEADER
#else
    std::shared_ptr<AudioTcpClient> m_spAudioTcpClient;
#endif


	XmlLoader m_ClientConfig;
	LocalizationLoader m_LocalizationConfig;

};

#endif // GlobalManager_h__
