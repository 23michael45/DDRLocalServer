#ifndef LocalTcpClient_h__
#define LocalTcpClient_h__

#include "../../Shared/src/Network/TcpClientBase.h"
#include "../../Shared/src/Utility/Singleton.h"

using namespace DDRFramework;
class LocalTcpClient : public TcpClientBase 
{
public:
	LocalTcpClient();
	~LocalTcpClient();

	void OnConnected(TcpSocketContainer& container) override;

	virtual std::shared_ptr<TcpClientSessionBase> BindSerializerDispatcher();

	auto shared_from_base() {
		return std::static_pointer_cast<LocalTcpClient>(shared_from_this());
	}
};


#endif // LocalTcpClient_h__
