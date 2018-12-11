#include "LocalServerHeadRuleRouter.h"



LocalServerHeadRuleRouter::LocalServerHeadRuleRouter()
{
}


LocalServerHeadRuleRouter::~LocalServerHeadRuleRouter()
{
}

bool LocalServerHeadRuleRouter::IgnoreBody(std::shared_ptr<TcpSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader) 
{
	
	return false;
};