
#include <memory>
#include "../../../Shared/proto/BaseCmd.pb.h"
#include "FileAddressProcessor.h"
#include "../../../Shared/src/Utility/DDRMacro.h"
#include "../../../Shared/src/Utility/Logger.h"
#include "../../../Shared/thirdparty/cppfs/include/cppfs/windows/LocalFileSystem.h"
#include "../../../Shared/thirdparty/cppfs/include/cppfs/FilePath.h"
#include "GlobalManager.h"
#include "../../../Shared/thirdparty/curl-asio/curl-asio.hpp"

#include <iostream>
#include <fstream>
using namespace DDRFramework;
using namespace DDRCommProto;



static curl_asio::data_action::type on_transfer_data_read(std::ofstream &out, const asio::const_buffer& buffer)
{
	out.write(asio::buffer_cast<const char*>(buffer), asio::buffer_size(buffer));
	return curl_asio::data_action::success;
}

static void on_transfer_done(curl_asio::transfer::ptr transfer, std::ofstream &out, const std::string &file, CURLcode result)
{
	if (result == CURLE_OK)
	{
		out.close();

		std::cout << "Transfer of " << transfer->info().effective_url() << " completed successfully (" << transfer->info().total_time() << " seconds)! Content saved to file " << file << std::endl;
		exit(0);
	}
	else
	{
		std::cerr << "Transfer of " << transfer->info().effective_url() << " failed with error " << result << std::endl;
		exit(1);
	}
}

void doGet(std::string& url,std::string outfile)
{
	asio::io_service io;
	curl_asio curl(io);
	curl_asio::transfer::ptr transfer = curl.create_transfer();
	if (transfer)
	{
		std::ofstream out(outfile);
		transfer->opt.protocols = CURLPROTO_HTTP | CURLPROTO_HTTPS;
		transfer->opt.max_redirs = 5;
		transfer->opt.redir_protocols = CURLPROTO_HTTP | CURLPROTO_HTTPS;
		transfer->opt.follow_location = true;
		transfer->on_data_read = std::bind(on_transfer_data_read, std::ref(out), std::placeholders::_1);
		transfer->on_done = std::bind(on_transfer_done, transfer, std::ref(out), outfile, std::placeholders::_1);
		if (transfer->start(url))
		{
			while (1)
				io.run();
		}
	}
}



FileAddressProcessor::FileAddressProcessor(BaseMessageDispatcher& dispatcher) :BaseProcessor(dispatcher)
{
}


FileAddressProcessor::~FileAddressProcessor()
{
}

void FileAddressProcessor::Process(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{

	rspFileAddress* pRaw = reinterpret_cast<rspFileAddress*>(spMsg.get());

	for (auto url : pRaw->fileaddrlist())
	{
		//do curl get;

		cppfs::FilePath path(cppfs::getexepath());
		auto root = path.directoryPath();


		cppfs::FilePath fpath(url);
		auto filename = fpath.fileName();


		doGet(url, root + filename);
	}


}

void FileAddressProcessor::AsyncProcess(std::shared_ptr<BaseSocketContainer> spSockContainer, std::shared_ptr<DDRCommProto::CommonHeader> spHeader, std::shared_ptr<google::protobuf::Message> spMsg)
{


}
