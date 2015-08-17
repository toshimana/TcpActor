#include "TcpBase.h"

#include <iostream>
#include <boost/thread/thread.hpp>

namespace asio     = boost::asio;
namespace ip       = asio::ip;

struct TcpBase::Impl
{
	std::unique_ptr<asio::io_service::work> work;
	boost::thread th;

	Impl( std::shared_ptr<asio::io_service> p )
		: work( new asio::io_service::work( *p ) )
		, th( [p](){ p->run(); } )
	{}

	~Impl()
	{
		// workの破棄
		work.reset();

		// threadの終了
		// shared_from_thisの効果により、
		// thスレッドのio_serviceのqueueの消化完了によって、
		// デコンストラクタが実施される可能性がある
		if ( boost::this_thread::get_id() != th.get_id() ) {
			th.join();
		}
	}
};


TcpBase::TcpBase()
	: io_service( std::make_shared<asio::io_service>() )
	, sock( *io_service )
	, mImpl( new TcpBase::Impl( io_service ) )
{
}

TcpBase::~TcpBase()
{
	
}

