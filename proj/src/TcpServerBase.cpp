#include "TcpServerBase.h"

#include <iostream>

#include <boost/bind/bind.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/thread.hpp>
#include <boost/signals2/signal.hpp>

namespace asio = boost::asio;
namespace ip   = asio::ip;
namespace sig  = boost::signals2;

namespace
{
	size_t getDataSize( const char* buf )
	{
		unsigned long size;
		memcpy( &size, buf + 4, 4 );
		return 8 + ntohl( size );
	}

	TcpBase::Message getMessage( const char* buf )
	{
		// header
		char type[4];
		unsigned long _size;
		memcpy( type,   buf,     4 );
		memcpy( &_size, buf + 4, 4 );
		size_t size = ntohl( _size );

		// data
		if ( memcmp( type, "txt", 4 ) == 0 ) {
			std::string txt( size, '\0' );
			memcpy( const_cast<char*>(txt.data()), buf + 8, size );
			return TcpBase::Text( txt );
		}
		else {
			std::cerr << __FUNCTION__ << " : unknown message" << std::endl;
			assert( false );
		}
		return TcpBase::Error();
	}
};

struct TcpServerBase::Impl
{
	struct ProcMessage : boost::static_visitor<void>
	{
		ProcMessage( TcpServerBase*const obj ) : base( obj ){}

		void operator()( const TcpBase::Error& value )
		{
			// do nothing.
		}

		void operator()( const TcpBase::Text& value )
		{
			base->mImpl->receiveMessage( value.msg );
		}
		TcpServerBase* const base;
	};

	TcpServerBase*const base;
	asio::ip::tcp::acceptor acceptor;
	boost::asio::streambuf receive_buffer;
	sig::signal<void( std::string )> receiveMessage;

	Impl( TcpServerBase*const obj, unsigned short port )
		: base( obj )
		, acceptor( *( base->io_service ), ip::tcp::endpoint(ip::tcp::v4(), port) )
	{
		// shared_from_thisはコンストラクタでは使用してはいけない
	}

	~Impl()
	{
		acceptor.close();
	}
};

TcpServerBase::TcpServerBase( unsigned short port )
	: mImpl(new TcpServerBase::Impl( this, port ) )
{
}


TcpServerBase::~TcpServerBase()
{
}

std::shared_ptr<TcpServerBase>
TcpServerBase::create( unsigned short port )
{
	return std::shared_ptr<TcpServerBase>( new TcpServerBase( port ) );
}

void
TcpServerBase::accept( void )
{
	std::shared_ptr<TcpServerBase> p = shared_from_this();
	mImpl->acceptor.async_accept( sock,
		[p](const boost::system::error_code& error){ p->on_accept(error); } );
}

void
TcpServerBase::connectReceiveMessage( std::function<void( std::string )> func )
{
	mImpl->receiveMessage.connect( func );
}

void 
TcpServerBase::on_accept( const boost::system::error_code& error )
{
	if ( error ) {
		std::cerr << __FUNCTION__ << " : " << error.message() << std::endl;
		return;
	}

	std::cout << __FUNCTION__ << " : connection success." << std::endl;
	receive();
}

void
TcpServerBase::on_receive( const boost::system::error_code& error, size_t bytes_transffered )
{
	if ( error && error != asio::error::eof ) {
		std::cerr << __FUNCTION__ << " : " << error.message() << std::endl;
		return;
	}

	// 受信量がヘッダサイズに満たなければ、次の受信を待つ
	while ( 8 < mImpl->receive_buffer.size() ) {
		std::cout << "Byte : " << mImpl->receive_buffer.size() << std::endl;
		const char* data = asio::buffer_cast<const char*>(mImpl->receive_buffer.data());
		size_t dataSize = getDataSize( data );

		// 受信量がデータサイズに満たなければ、次の受信を待つ
		if ( mImpl->receive_buffer.size() < dataSize ) break;

		// メッセージを解析する
		TcpBase::Message msg = getMessage( data );
		mImpl->receive_buffer.consume( dataSize );
		TcpServerBase::Impl::ProcMessage pm( this );
		apply_visitor( pm, msg );
	}
	receive();
}

void
TcpServerBase::receive()
{
	std::shared_ptr<TcpServerBase> p = shared_from_this();
	// async_readの第3引数はtransfer_allにすると、
	// いつまでも受信待ちになる。
	// transfer_at_leastで最低受信サイズを設定した方が良い？
	asio::async_read( sock, 
		mImpl->receive_buffer,
		asio::transfer_at_least(1),
		[p](const boost::system::error_code& error, size_t bytes_transffered){ p->on_receive(error, bytes_transffered); } );
}