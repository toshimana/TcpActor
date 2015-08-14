#include "TcpBase.h"

#include <iostream>

namespace asio     = boost::asio;
namespace ip       = asio::ip;

TcpBase::TcpBase()
	: io_service()
	, sock( io_service )
{
}

TcpBase::~TcpBase()
{
}

void
TcpBase::sendMessage( const std::string& msg )
{
	std::cout << __FUNCTION__ << std::endl;
	asio::write( sock, asio::buffer( msg ) );
}

std::string
TcpBase::receiveMessage()
{
	std::cout << __FUNCTION__ << std::endl;
	asio::streambuf receive_buffer;
	boost::system::error_code error;
	asio::read( this->sock, receive_buffer, asio::transfer_all(), error );
	if ( error && error != asio::error::eof ) {
		std::cerr << error.message() << std::endl;
	}
	return asio::buffer_cast<const char*>(receive_buffer.data());
}
