#ifndef RemoteServerHeadRuleRouter_h__
#define RemoteServerHeadRuleRouter_h__


#include "src/Network/BaseProcessor.h"
#include "asio.hpp"
using namespace DDRFramework;
class RemoteServerHeadRuleRouter : public BaseHeadRuleRouter
{
public:
	RemoteServerHeadRuleRouter();
	~RemoteServerHeadRuleRouter();

	virtual bool IgnoreBody(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, asio::streambuf& buf, int bodylen) override;
};


#endif // RemoteServerHeadRuleRouter_h__