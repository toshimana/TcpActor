#pragma once

#include "TcpBase.h"
#include <functional>

class TcpServerBase : public TcpBase, public std::enable_shared_from_this<TcpServerBase>
{
public:
	~TcpServerBase();

	void accept();
	void connectReceiveMessage( std::function<void( std::string )> func );

	static std::shared_ptr<TcpServerBase> create( unsigned short port );

private:
	struct Impl;
	std::unique_ptr<Impl> mImpl;

	TcpServerBase( unsigned short port );

	void on_accept( const boost::system::error_code& error );
	void on_receive( const boost::system::error_code& error, size_t bytes_transffered );
	void receive( void );
};

