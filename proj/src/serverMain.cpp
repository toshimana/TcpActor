#include "TcpServerBase.h"
#include <iostream>

int main( int argc, char* argv[] )
{
	unsigned short port( 30 );

	std::shared_ptr<TcpServerBase> server = TcpServerBase::create( port );
	server->connectReceiveText( []( const std::string& msg ){ 
		std::cout << msg << std::endl; } );
	server->accept();

	while ( 1 ){}

	return 0;
}