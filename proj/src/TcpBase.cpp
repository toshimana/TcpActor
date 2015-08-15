#include "TcpBase.h"

#include <iostream>

namespace asio     = boost::asio;
namespace ip       = asio::ip;

TcpBase::TcpBase()
	: io_service( std::make_shared<asio::io_service>() )
	, sock( *io_service )
{
}

TcpBase::~TcpBase()
{
}

void
TcpBase::sendMessage( const std::string& msg )
{
}

std::string
TcpBase::receiveMessage()
{
	return std::string();
}

