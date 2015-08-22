#pragma once

#include <boost/asio.hpp>
#include <boost/variant.hpp>

class TcpBase
{
public:
	std::shared_ptr<boost::asio::io_service> io_service;
	boost::asio::ip::tcp::socket sock;

	TcpBase();
	virtual ~TcpBase();

	struct Error
	{
		Error( int _errorno = -1 ) : errorno( _errorno ) {}
		const int errorno;
	};

	struct Text
	{
		Text( const std::string& _msg )	: msg( _msg ){}
		const std::string msg;
	};

	typedef boost::variant<Error,Text> Message;


private:
	struct Impl;
	std::unique_ptr<Impl> mImpl;
};
