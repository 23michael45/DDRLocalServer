#ifndef GlobalManager_h__
#define GlobalManager_h__

#ifdef QT_PRECOMPILED_HEADER
    #include "stable.h"
#else
    #include "AudioTcpClient.h"
#endif

#include "src/Network/UdpSocketBase.h"
#include "src/Network/TcpClientBase.h"
#include "src/Utility/Singleton.h"
#include "LocalTcpClient.h"
#include "src/Utility/LocalizationLoader.h"
#include "src/Utility/GlobalManagerBase.h"

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


    std::shared_ptr<AudioTcpClient> GetAudioTcpClient();

	void SetServerAddr(std::string ip, std::string port);

	virtual void TcpConnect(std::string ip, std::string port) override;
	void TcpConnect();




	void ConnectBroadcastServer();

	void CloseBroadcastServer();

	void AssignRemoteLoginInfo(rspRemoteLogin info)
	{
		m_RemoteLoginLSInfo.CopyFrom(info);
	}
	rspRemoteLogin& GetRemoteLoginInfo()
	{
		return m_RemoteLoginLSInfo;
	}

private:

	std::shared_ptr<AudioTcpClient> m_spAudioTcpClient;
	std::shared_ptr<LSBroadcastReceiveTcpClient> m_spLSBroadcastReceiveTcpClient;



	XmlLoader m_Config;

	std::string m_ServerIP;
	std::string m_ServerPort;

	rspRemoteLogin m_RemoteLoginLSInfo;

};

#endif // GlobalManager_h__
