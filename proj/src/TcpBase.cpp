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
		// work�̔j��
		work.reset();

		// thread�̏I��
		// shared_from_this�̌��ʂɂ��A
		// th�X���b�h��io_service��queue�̏��������ɂ���āA
		// �f�R���X�g���N�^�����{�����\��������
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

