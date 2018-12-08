#ifndef LocalTcpServer_h__
#define LocalTcpServer_h__

#include "../../Shared/src/Network/TcpServerBase.h"

using namespace DDRFramework;
class LocalTcpServer : public TcpServerBase
{
public:
	LocalTcpServer(int port);
	~LocalTcpServer();


	virtual std::shared_ptr<TcpSessionBase> BindSerializerDispatcher();

	auto shared_from_base() {
		return std::static_pointer_cast<LocalTcpServer>(shared_from_this());
	}
};


#endif // LocalTcpServer_h__