#pragma once

#include "TcpBase.h"

class TcpClientBase : public TcpBase
{
public:
	TcpClientBase( const std::string& address, unsigned short port );
	~TcpClientBase();
};

