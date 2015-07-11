#include "TcpClientBase.h"

#include <iostream>
#include "TcpBase.h"

namespace asio     = boost::asio;
namespace ip       = asio::ip;

struct TcpClientBase::Impl
{
	TcpBase base;

	Impl( const std::string& address, unsigned short port )
		: base()
	{
		base.sock.connect( ip::tcp::endpoint( ip::address::from_string( address ), port ) );
	}

};


TcpClientBase::TcpClientBase( const std::string& address, unsigned short port )
	: mImpl(new TcpClientBase::Impl( address, port ) )
{
}


TcpClientBase::~TcpClientBase()
{
}

void 
TcpClientBase::sendMessage( const std::string& msg, std::function<void(std::string)> rcv ) const
{
	// send
	mImpl->base.sendMessage( msg );

	// receive
	rcv( mImpl->base.receiveMessage() );
}
