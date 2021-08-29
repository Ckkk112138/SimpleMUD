#ifndef TRAIN_H
#define TRAIN_H

#include <string>
#include "SocketLib.h"
#include "PlayerDatabase.h"

using std::string;
using namespace SocketLib;

namespace SimpleMUD
{
	class Train :public Telnet::handler
	{
	protected:
		player m_player;
	public:
		Train(Connection<Telnet>& p_conn,player p_player)
			:Telnet::handler(p_conn)
		{
			m_player = p_player;
		}

		void Handle(string p_data);
		void Enter();
		void Leave(){}
		void Hungup() { PlayerDatabase::Logout(m_player); }
		void Flooded() { PlayerDatabase::Logout(m_player); }
		void PrintStats(bool p_clear = true);
	};
}
#endif // !TRAIN_H

