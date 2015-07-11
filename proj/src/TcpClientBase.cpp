#include "TcpClientBase.h"

#include <iostream>

namespace asio     = boost::asio;
namespace ip       = asio::ip;

TcpClientBase::TcpClientBase( const std::string& address, unsigned short port )
{
	sock.connect( ip::tcp::endpoint( ip::address::from_string( address ), port ) );
}


TcpClientBase::~TcpClientBase()
{
}

