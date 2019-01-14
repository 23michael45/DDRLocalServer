#ifndef UploadFileProgressProcessor_h__
#define UploadFileProgressProcessor_h__


#include "../../../Shared/src/Network/MessageSerializer.h"
#include "../../../Shared/src/Network/BaseProcessor.h"


using namespace DDRFramework;
class UploadFileProgressProcessor : public BaseProcessor
{
public:
	UploadFileProgressProcessor(BaseMessageDispatcher& dispatcher);
	~UploadFileProgressProcessor();

	virtual void Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg) override;
	virtual void AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg) override;
private:


	CommonHeader_PassNode m_Passnode;
	std::shared_ptr<DDRCommProto::CommonHeader> m_spHeader;
};

#endif // UploadFileProgressProcessor_h__

