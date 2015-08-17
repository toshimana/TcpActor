#pragma once

#include "TcpBase.h"

class TcpClientBase : public TcpBase, public std::enable_shared_from_this<TcpClientBase>
{
public:
	TcpClientBase( const std::string& address, unsigned short port );
	virtual ~TcpClientBase();

	virtual void sendMessage( const std::string& msg );
};

