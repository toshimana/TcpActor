#include "TcpServerBase.h"

#include <iostream>
#include "TcpBase.h"

namespace asio     = boost::asio;
namespace ip       = asio::ip;

struct TcpServerBase::Impl
{
	TcpBase base;
	ip::tcp::acceptor acceptor;

	Impl( unsigned short port )
		: base()
		, acceptor( base.io_service, ip::tcp::endpoint(ip::tcp::v4(), port ) )
	{
		acceptor.accept( base.sock );
	}

};


TcpServerBase::TcpServerBase( unsigned short port )
	: mImpl( new TcpServerBase::Impl( port ) )
{
}


TcpServerBase::~TcpServerBase()
{
}

void
TcpServerBase::receiveMessage( void ) const
{
	// receive
	mImpl->base.receiveMessage();

}
