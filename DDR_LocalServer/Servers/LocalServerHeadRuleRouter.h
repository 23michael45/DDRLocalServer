#ifndef LocalServerHeadRuleRouter_h__
#define LocalServerHeadRuleRouter_h__
#include "../../../Shared/src/Network/BaseProcessor.h"
using namespace DDRFramework;
class LocalServerHeadRuleRouter : public BaseHeadRuleRouter
{
public:
	LocalServerHeadRuleRouter();
	~LocalServerHeadRuleRouter();

	virtual bool IgnoreBody(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader) override;
};


#endif // LocalServerHeadRuleRouter_h__