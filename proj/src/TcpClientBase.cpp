#include "TcpClientBase.h"

#include <iostream>

#include <boost/bind/bind.hpp>

namespace asio     = boost::asio;
namespace ip       = asio::ip;

namespace
{
	struct Message
	{
		std::string msg;

		Message( const std::string& _msg )
			: msg( _msg )
		{}

		void
		on_send( const boost::system::error_code& error, size_t bytes_transffered ) const
		{
			if ( error ) {
				std::cerr << __FUNCTION__ << " : " << error.message() << std::endl;
				return;
			}
			std::cout << "[OUT]" << "send success." << std::endl;
		}
	};
};

TcpClientBase::TcpClientBase( const std::string& address, unsigned short port )
{
	sock.connect( ip::tcp::endpoint( ip::address::from_string(address), port) );
}


TcpClientBase::~TcpClientBase()
{
}

void
TcpClientBase::sendMessage( const std::string& msg )
{
	// Messageオブジェクトは送信処理が終了した時点で解放される
	std::shared_ptr<Message> p = std::make_shared<Message>( msg );
	asio::async_write( sock, 
		asio::buffer( p->msg ),
		[p]( const boost::system::error_code& error, size_t bytes_transffered ){ 
		p->on_send( error, bytes_transffered ); }
		);
}

