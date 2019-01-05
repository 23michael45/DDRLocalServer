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
class GlobalManager : public DDRFramework::CSingleton<GlobalManager> , public GlobalManagerClientBase
{
public:
	GlobalManager();
	~GlobalManager();

public:

	virtual void Init() override;
	virtual bool StartUdp() override;

	void StartAudioClient(std::string ip, int port);
	void StopAudioClient();



	
#ifdef QT_PRECOMPILED_HEADER
#else
    std::shared_ptr<AudioTcpClient> GetAudioTcpClient();
#endif

	void SetServerAddr(std::string ip, std::string port);

	virtual void TcpConnect(std::string ip, std::string port) override;
	void TcpConnect();

private:

#ifdef QT_PRECOMPILED_HEADER
#else
    std::shared_ptr<AudioTcpClient> m_spAudioTcpClient;
#endif


	XmlLoader m_Config;

	std::string m_ServerIP;
	std::string m_ServerPort;

};

#endif // GlobalManager_h__
