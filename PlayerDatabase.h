#ifndef PLAYERDATABASE_H
#define PLAYERDATABASE_H

#include "Player.h"
#include "EntityDatabase.h"
#include <string>
#include "SimpleMUDLogs.h"
#include <iostream>

namespace SimpleMUD
{
	class PlayerDatabase:public EntityDatabase<Player>
	{
	public:
		static bool Load();
		static bool Save();
		static bool AddPlayer(Player& p);
		static std::string PlayerFileName(const std::string& p_name)
		{
			return std::string("players/" + p_name + ".plr");
		}
		static void LoadPlayer(std::string p_name);
		static void SavePlayer(entityid p_player);
		static entityid LastID()
		{
			entityid id = m_map.rbegin()->first;
			return id;
		}
		static iterator findactive(const std::string& p_name);
		static iterator findloggedin(const std::string& p_name);
		static void Logout(entityid p_player);
		static void  PrintPlayer()
		{
			for (int i = 0; i < m_map.size(); ++i)
			{
				std::cout << m_map[i] << "\n";
			}
		}
		static void  PrintPlayer(int i)
		{
			std::cout << m_map[i] << "\n";

		}

	};

	
}
#endif // !PLAYERDATABASE_H

