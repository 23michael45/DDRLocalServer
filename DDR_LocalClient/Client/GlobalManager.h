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
#include "../../../Shared/src/Utility/GlobalManagerBase.h"

using namespace DDRFramework;
class GlobalManager : public DDRFramework::CSingleton<GlobalManager> , public GlobalManagerBase
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


	
	std::shared_ptr<LocalTcpClient> GetTcpClient();
    std::shared_ptr<UdpSocketBase> GetUdpClient();
#ifdef QT_PRECOMPILED_HEADER
#else
    std::shared_ptr<AudioTcpClient> GetAudioTcpClient();
#endif

	void SetServerAddr(std::string ip, std::string port);
	void TryConnect();
	void TryConnect(std::string ip, std::string port);
private:

	void OnUdpDisconnect(UdpSocketBase& container);

	std::shared_ptr<LocalTcpClient> m_spTcpClient;
	std::shared_ptr<UdpSocketBase> m_spUdpClient;
#ifdef QT_PRECOMPILED_HEADER
#else
    std::shared_ptr<AudioTcpClient> m_spAudioTcpClient;
#endif


	XmlLoader m_ClientConfig;

	std::string m_ServerIP;
	std::string m_ServerPort;

};

#endif // GlobalManager_h__
