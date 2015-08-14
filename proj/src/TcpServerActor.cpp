#include "TcpServerActor.h"

#include <future>

#include <boost/signals2/signal.hpp>
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/back/state_machine.hpp>

#include "TcpServerBase.h"

struct TcpServerActor::Impl
{
	struct Machine : boost::msm::front::state_machine_def < Machine >
	{
		Machine(TcpServerActor* obj):base(obj){}

		// í‚é~ñΩóﬂ
		struct Halt {
			Halt( std::shared_ptr<std::promise<void> > _msg ) : msg( _msg ){};
			std::shared_ptr<std::promise<void> > msg;
		};
		struct HaltState : boost::msm::front::state < > {};
		void on_halt( const Halt& msg )
		{
			msg.msg->set_value();
		}

		// State
		struct StopState : boost::msm::front::state < > 
		{
			template <class Event, class Fsm>
			void on_entry( Event const& evt, Fsm& fsm )
			{
				if ( fsm.base->mImpl->server )
				{
					fsm.base->mImpl->server.reset();
				}
			}
		};

		struct ReceiveState : boost::msm::front::state < > 
		{
			template <class Event, class Fsm>
			void on_entry( Event const& evt, Fsm& fsm )
			{
				fsm.base->entry( TcpServerActorMessage::Receive() );
			}
		};

		// ëJà⁄ñΩóﬂ
		bool on_initialize( const TcpServerActorMessage::Initialize& msg )
		{
			base->mImpl->server = std::make_unique<TcpServerBase>( msg.port );
			return true;
		}

		void on_receive( const TcpServerActorMessage::Receive& )
		{
			base->entry( TcpServerActorMessage::Receive() );

			std::cout << __FUNCTION__ << std::endl;
			std::string msg = base->mImpl->server->receiveMessage();
			std::cout << __FUNCTION__ << std::endl;
			if ( msg.empty() ) return;

			base->mImpl->receiveMessage( msg );
		}

		// Transition
		struct transition_table : boost::mpl::vector <
			a_row < HaltState,    Halt,   HaltState, &TcpServerActor::Impl::Machine::on_halt >,
			a_row < StopState,    Halt,   HaltState, &TcpServerActor::Impl::Machine::on_halt >,
			a_row < ReceiveState, Halt,   HaltState, &TcpServerActor::Impl::Machine::on_halt >,
			g_row < StopState,    TcpServerActorMessage::Initialize, ReceiveState, &TcpServerActor::Impl::Machine::on_initialize >,
			a_row < ReceiveState, TcpServerActorMessage::Receive,    ReceiveState, &TcpServerActor::Impl::Machine::on_receive >
			> {};

		typedef StopState initial_state;

	protected:
		template <class FSM, class Event>
		void no_transition( Event const&, FSM&, int )
		{
			// do nothing.
		}

	private:
		TcpServerActor* base;
	};


	class MessageVisitor : public boost::static_visitor < void >
	{
	public:
		MessageVisitor( TcpServerActor* const obj ) : base( obj ){}

		template<typename T>
		void operator()( const T& msg ){
			base->mImpl->machine.process_event( msg );
		}
	private:
		TcpServerActor* base;
	};

	std::unique_ptr<TcpServerBase> server;
	boost::msm::back::state_machine<Machine> machine;

	boost::signals2::signal<void( std::string )> receiveMessage;

	Impl( TcpServerActor* const obj)
		: machine( obj )
		, server()
	{
	}

	~Impl()
	{
		std::shared_ptr<std::promise<void> > p = std::make_shared<std::promise<void> >();
		std::future<void> f = p->get_future();
		machine.process_event( Machine::Halt( p ) );

		f.get();
	}
};

TcpServerActor::TcpServerActor()
	: mImpl( new TcpServerActor::Impl( this ) )
{
	mImpl->machine.start();
}


TcpServerActor::~TcpServerActor()
{
}

void 
TcpServerActor::connectReceiveMessage( std::function<void( std::string )> func )
{
	mImpl->receiveMessage.connect( func );
}


void 
TcpServerActor::processMessage( std::shared_ptr<TcpServerActorMessage::Message> msg )
{
	Impl::MessageVisitor mv( this );
	boost::apply_visitor( mv, *msg );
}
