#pragma once

#include <boost/variant.hpp>

#include <Actor.hpp>
#include <memory>

namespace TcpServerActorMessage
{
	struct Initialize
	{
		Initialize( unsigned short _port ) : port( _port ){};
		unsigned short port;
	};

	struct Receive {};
	
	typedef boost::variant<Initialize,Receive> Message;
};

class TcpServerActor : public Actor<TcpServerActorMessage::Message>
{
public:
	TcpServerActor();
	~TcpServerActor();

	void connectReceiveMessage( std::function<void( std::string )> func );

private:
	struct Impl;
	std::unique_ptr<Impl> mImpl;

	void processMessage( std::shared_ptr<TcpServerActorMessage::Message> msg );
};
