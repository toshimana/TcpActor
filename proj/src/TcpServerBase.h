#pragma once

#include "TcpBase.h"

class TcpServerBase : public TcpBase, public std::enable_shared_from_this<TcpServerBase>
{
public:
	TcpServerBase( unsigned short port );
	~TcpServerBase();

	void accept();

	virtual void on_accept( const boost::system::error_code& error );
	virtual void on_receive( const boost::system::error_code& error, size_t bytes_transffered );

private:
	struct Impl;
	std::unique_ptr<Impl> mImpl;
};

