#include "TcpClientBase.h"

#include <cstdlib>
#include <iostream>

#include <boost/bind/bind.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>

namespace asio     = boost::asio;
namespace ip       = asio::ip;

namespace
{
	struct Msg2Binary : boost::static_visitor<void>
	{
		std::vector<unsigned char> buf;

		void operator()( const TcpBase::Error& value )
		{
			// ここに来たら作りが悪い
			assert( false );
		}

		void operator()( const TcpBase::Text& value )
		{
			size_t bufSize = 4 + 4 + value.msg.size();
			buf.resize( bufSize, '\0' );
			unsigned long dataSize = htonl( value.msg.size() );

			// header ( Type + Size )
			memcpy( buf.data(), "txt",     3 );
			memcpy( buf.data() + 4, &dataSize, 4 );

			// data
			memcpy( buf.data() + 8, value.msg.data(), value.msg.size() );
		}

		void operator()( const TcpBase::Image& value )
		{
			std::vector<uchar> imgBuf;
			cv::imencode( ".png", value.img, imgBuf );

			size_t bufSize = 4 + 4 + imgBuf.size();
			buf.resize( bufSize, '\0' );
			unsigned long dataSize = htonl( imgBuf.size() );

			// header
			memcpy( buf.data()    , "img",     3 );
			memcpy( buf.data() + 4, &dataSize, 4 );

			// data
			memcpy( buf.data() + 8, imgBuf.data(), imgBuf.size() );
		}
	};

	struct SMessage
	{
		std::vector<unsigned char> msg;

		SMessage( const std::vector<unsigned char>& _msg )
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

std::shared_ptr<TcpClientBase>
TcpClientBase::create( const std::string& address, unsigned short port )
{
	return std::shared_ptr<TcpClientBase>( new TcpClientBase( address, port ) );
}

void
TcpClientBase::sendMessage( const Message& msg )
{
	Msg2Binary m2b;
	apply_visitor( m2b, msg );

	// SMessageオブジェクトは送信処理が終了した時点で解放される
	std::shared_ptr<SMessage> p = std::make_shared<SMessage>( m2b.buf );
	asio::async_write( sock, 
		asio::buffer( p->msg ),
		[p]( const boost::system::error_code& error, size_t bytes_transffered ){ 
		p->on_send( error, bytes_transffered ); }
		);
}

