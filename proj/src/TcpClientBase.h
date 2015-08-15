#pragma once

#include "TcpBase.h"

class TcpClientBase : public TcpBase
{
public:
	TcpClientBase( const std::string& address, unsigned short port );
	virtual ~TcpClientBase();

	virtual void sendMessage( const std::string& msg );

protected:
	virtual void on_accept( const boost::system::error_code& error );
	virtual void on_send( const boost::system::error_code& error, size_t bytes_transffered ) const;
};

