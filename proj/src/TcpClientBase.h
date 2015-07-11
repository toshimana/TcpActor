#pragma once

#include <string>
#include <memory>
#include <functional>

class TcpClientBase
{
public:
	TcpClientBase( const std::string& address, unsigned short port );
	~TcpClientBase();

	void sendMessage( const std::string& msg, std::function<void(std::string)> rcv ) const;

private:
	struct Impl;
	std::unique_ptr<Impl> mImpl;
};

