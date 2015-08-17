#pragma once

#include <boost/asio.hpp>

class TcpBase
{
public:
	std::shared_ptr<boost::asio::io_service> io_service;
	boost::asio::ip::tcp::socket sock;

	TcpBase();
	virtual ~TcpBase();

private:
	struct Impl;
	std::unique_ptr<Impl> mImpl;
};
