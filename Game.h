#ifndef GAME_H
#define GAME_H

#include "SocketLib.h"
#include "BasicLib.h"
#include <string>
#include "PlayerDatabase.h"
#include "ItemDatabase.h"
#include "Train.h"
#include "Logon.h"

using namespace BasicLib;
using namespace SocketLib;
using std::string;

namespace SimpleMUD
{
	class Game :public Telnet::handler
	{
	protected:
		player m_player;
		string m_lastcommand;
		static Timer s_timer;
		static bool isRunning;
	public:
		void Handle(string p_data);
		void Enter();
		void Leave();
		void Hungup();
		void Flooded();

		static void SendGlobal(const string& p_str);
		static void SendGame(const string& p_str);
		static void Announce(const string& p_announcement);
		static void LogoutMessage(const string& p_reason);
		void Whisper(string p_str, string p_player);

		static string WhoList(const string& p_who);
		static string PrintHelp(PlayerRank p_rank = REGULAR);
		string PrintStats();
		string PrintExperience();
		string PrintInventory();

		bool UseItem(const string& p_item);
		bool RemoveItem(string p_item);
		static BasicLib::Timer& GetTimer() { return s_timer; }
		static bool& Running() { return isRunning; }
		Game(Connection<Telnet>& p_conn, player p_player)
			:Telnet::handler(p_conn)
		{
			m_player = p_player;
		}
		//static void Logout(player p_player);
		void GotoTrain();
	};
}
#endif // !GAME_H

