#include "TcpServerBase.h"

#include <iostream>

#include <boost/bind/bind.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/thread.hpp>

namespace asio     = boost::asio;
namespace ip       = asio::ip;

struct TcpServerBase::Impl
{
	TcpServerBase*const base;
	asio::ip::tcp::acceptor acceptor;
	boost::asio::streambuf receive_buffer;

	Impl( TcpServerBase*const obj, unsigned short port )
		: base( obj )
		, acceptor( *( base->io_service ), ip::tcp::endpoint(ip::tcp::v4(), port) )
	{
		// shared_from_thisはコンストラクタでは使用してはいけない
	}

	~Impl()
	{
	}
};

TcpServerBase::TcpServerBase( unsigned short port )
	: mImpl(new TcpServerBase::Impl( this, port ) )
{
}


TcpServerBase::~TcpServerBase()
{
}

void
TcpServerBase::accept( void )
{
	std::shared_ptr<TcpServerBase> p = shared_from_this();
	mImpl->acceptor.async_accept( sock,
		[p](const boost::system::error_code& error){ p->on_accept(error); } );
}

void 
TcpServerBase::on_accept( const boost::system::error_code& error )
{
	if ( error ) {
		std::cerr << __FUNCTION__ << " : " << error.message() << std::endl;
		return;
	}

	std::cout << __FUNCTION__ << " : connection success." << std::endl;

	std::shared_ptr<TcpServerBase> p = shared_from_this();
	asio::async_read( sock, 
		mImpl->receive_buffer,
		asio::transfer_all(),
		[p](const boost::system::error_code& error, size_t bytes_transffered){ p->on_receive(error, bytes_transffered); } );
}


void
TcpServerBase::on_receive( const boost::system::error_code& error, size_t bytes_transffered )
{
	if ( error && error != asio::error::eof ) {
		std::cerr << __FUNCTION__ << " : " << error.message() << std::endl;
		return;
	}

	const char* m = asio::buffer_cast<const char*>(mImpl->receive_buffer.data());
	std::string msg( m );
	std::cout << __FUNCTION__ << " : " << msg << std::endl;
}
