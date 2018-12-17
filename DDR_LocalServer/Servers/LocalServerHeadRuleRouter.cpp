#include "LocalServerHeadRuleRouter.h"



LocalServerHeadRuleRouter::LocalServerHeadRuleRouter()
{
}


LocalServerHeadRuleRouter::~LocalServerHeadRuleRouter()
{
}

bool LocalServerHeadRuleRouter::IgnoreBody(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader)
{
	
	return false;
};