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
	std::unique_ptr<asio::io_service::work> work;
	asio::ip::tcp::acceptor acceptor;
	boost::thread th;
	Impl( TcpServerBase*const obj, unsigned short port )
		: base( obj )
		, work( new asio::io_service::work( *(base->io_service) ) )
		, acceptor( *( base->io_service ), ip::tcp::endpoint(ip::tcp::v4(), port) )
		, th( [this](){ base->io_service->run(); } )
	{
		acceptor.async_accept( base->sock,
			boost::bind( &TcpServerBase::on_accept, base, asio::placeholders::error ) );
	}

	~Impl()
	{
		// workÇÃîjä¸
		work.reset();

		// threadÇÃèIóπ
		th.join();
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
TcpServerBase::on_accept( const boost::system::error_code& error )
{
	if ( error ) {
		std::cerr << __FUNCTION__ << " : " << error.message() << std::endl;
		return;
	}

	std::cout << __FUNCTION__ << " : connection success." << std::endl;

	asio::async_read( this->sock, 
		receive_buffer,
		asio::transfer_all(),
		boost::bind( &TcpServerBase::on_receive, this, asio::placeholders::error, asio::placeholders::bytes_transferred ) );
}


void
TcpServerBase::on_receive( const boost::system::error_code& error, size_t bytes_transffered )
{
	if ( error && error != asio::error::eof ) {
		std::cerr << __FUNCTION__ << " : " << error.message() << std::endl;
		return;
	}

	const char* m = asio::buffer_cast<const char*>(receive_buffer.data());
	std::string msg( m );
	std::cout << __FUNCTION__ << " : " << msg << std::endl;
}
