#pragma once

#include <boost/asio.hpp>

class TcpBase
{
public:
	std::shared_ptr<boost::asio::io_service> io_service;
	boost::asio::ip::tcp::socket sock;

	TcpBase();
	virtual ~TcpBase();

	virtual void sendMessage( const std::string& msg );
	virtual std::string receiveMessage();

protected:
	virtual void on_accept( const boost::system::error_code& error ) = 0;
};
