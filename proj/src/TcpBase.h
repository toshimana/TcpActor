#pragma once

#include <boost/asio.hpp>

class TcpBase
{
public:
	boost::asio::io_service      io_service;
	boost::asio::ip::tcp::socket sock;

	TcpBase();
	~TcpBase();

	void sendMessage( const std::string& msg ) const;
	std::string receiveMessage();
};

