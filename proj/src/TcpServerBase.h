#pragma once

#include <memory>

class TcpServerBase
{
public:
	TcpServerBase( unsigned short port );
	~TcpServerBase();

	void receiveMessage( void ) const;

private:
	struct Impl;
	std::unique_ptr<Impl> mImpl;
};

