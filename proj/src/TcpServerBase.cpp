#include "TcpServerBase.h"

#include <iostream>

namespace asio     = boost::asio;
namespace ip       = asio::ip;


TcpServerBase::TcpServerBase( unsigned short port )
	: acceptor( io_service, ip::tcp::endpoint( ip::tcp::v4(), port ) )
{
	acceptor.accept( sock );
}


TcpServerBase::~TcpServerBase()
{
}

