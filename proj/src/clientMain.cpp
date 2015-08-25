#include "TcpClientBase.h"

int main( int argc, char* argv[] )
{
	std::shared_ptr<TcpClientBase> client = TcpClientBase::create( "127.0.0.1", 30 );

	while ( 1 ){
		std::string message;
		std::cin >> message;
		client->sendMessage( message );
	}
	return 0;
}