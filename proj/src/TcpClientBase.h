#pragma once

#include "TcpBase.h"

class TcpClientBase : public TcpBase, public std::enable_shared_from_this<TcpClientBase>
{
public:
	virtual ~TcpClientBase();
	static std::shared_ptr<TcpClientBase> create( const std::string& address, unsigned short port );

	virtual void sendMessage( const Message& msg );

private:
	TcpClientBase( const std::string& address, unsigned short port );
};

