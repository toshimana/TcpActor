#pragma once

#include "TcpBase.h"

class TcpServerBase : public TcpBase
{
public:
	TcpServerBase( unsigned short port );
	~TcpServerBase();

	boost::asio::ip::tcp::acceptor acceptor;
};

