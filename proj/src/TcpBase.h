#pragma once

#include <boost/asio.hpp>

class TcpBase
{
public:
	boost::asio::io_service      io_service;
	boost::asio::ip::tcp::socket sock;

	TcpBase();
	virtual ~TcpBase();

	virtual void sendMessage( const std::string& msg );
	virtual std::string receiveMessage();
};

