#include "TcpClientBase.h"

#include <cstdlib>
#include <iostream>

#include <boost/bind/bind.hpp>

namespace asio     = boost::asio;
namespace ip       = asio::ip;

namespace
{
	struct Msg2Binary : boost::static_visitor<std::string>
	{
		std::string operator()( const TcpBase::Error& value )
		{
			// ここに来たら作りが悪い
			assert( false );
			return "";
		}

		std::string operator()( const TcpBase::Text& value )
		{
			size_t bufSize = 4 + 4 + value.msg.size();
			std::string buf( bufSize, '\0' );
			unsigned long dataSize = htonl( value.msg.size() );

			// header ( Type + Size )
			memcpy( const_cast<char*>(buf.data()    ), "txt",     3 );
			memcpy( const_cast<char*>(buf.data() + 4), &dataSize, 4 );

			// data
			memcpy( const_cast<char*>(buf.data() + 8), value.msg.data(), value.msg.size() );

			return buf;
		}
	};

	struct SMessage
	{
		std::string msg;

		SMessage( const std::string& _msg )
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
	boost::system::error_code ec;
	sock.connect( ip::tcp::endpoint( ip::address::from_string(address), port), ec );
	if ( ec ) {
		std::cerr << __FUNCTION__ << ec.message() << std::endl;
	}
}


TcpClientBase::~TcpClientBase()
{
}

void
TcpClientBase::sendMessage( const Message& msg )
{
	Msg2Binary m2b;
	std::string msgBinary = apply_visitor( m2b, msg );

	// SMessageオブジェクトは送信処理が終了した時点で解放される
	std::shared_ptr<SMessage> p = std::make_shared<SMessage>( msgBinary );
	asio::async_write( sock, 
		asio::buffer( p->msg ),
		[p]( const boost::system::error_code& error, size_t bytes_transffered ){ 
		p->on_send( error, bytes_transffered ); }
		);
}

