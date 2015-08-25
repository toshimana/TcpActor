#include "TcpServerBase.h"

int main( int argc, char* argv[] )
{
	unsigned short port( 30 );

	std::shared_ptr<TcpServerBase> server = TcpServerBase::create( port );
	server->connectReceiveMessage( []( const std::string& msg ){ 
		std::cout << msg << std::endl; } );
	server->accept();

	while ( 1 ){}

	return 0;
}