#include "TcpClientBase.h"

#include <iostream>

#include <boost/bind/bind.hpp>

namespace asio     = boost::asio;
namespace ip       = asio::ip;

TcpClientBase::TcpClientBase( const std::string& address, unsigned short port )
{
	sock.async_connect( ip::tcp::endpoint( ip::address::from_string( address ), port ), 
		boost::bind( &TcpClientBase::on_accept, this, asio::placeholders::error ) );
	io_service->run();
}


TcpClientBase::~TcpClientBase()
{
}

void
TcpClientBase::sendMessage( const std::string& msg )
{
	io_service->reset();
	asio::async_write( sock, 
		asio::buffer( msg ),
		boost::bind( &TcpClientBase::on_send, this, asio::placeholders::error, asio::placeholders::bytes_transferred )
		);
	io_service->run();
}

void 
TcpClientBase::on_accept( const boost::system::error_code& error )
{
	if ( error ) {
		std::cerr << __FUNCTION__ << " : " << error.message() << std::endl;
		return;
	}

	std::cout << __FUNCTION__ << " : connection success." << std::endl;
}

void 
TcpClientBase::on_send( const boost::system::error_code& error, size_t bytes_transffered ) const
{
	if ( error ) {
		std::cerr << __FUNCTION__ << " : " << error.message() << std::endl;
		return;
	}
	std::cout << "[OUT]" << "send success." << std::endl;
}

