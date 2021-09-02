#include "PlayerDatabase.h"
#include "BasicLib.h"
#include <fstream>
#include <map>
#include "SocketLib.h"

using namespace BasicLib;
using std::map;
using std::ifstream;
using std::ofstream;
using std::string;


namespace SimpleMUD
{
	std::map<entityid, Player> EntityDatabase<Player>::m_map;

	bool PlayerDatabase::Load()
	{
		ifstream file("players/players.txt");
		string name;
		while (!file.eof())
		{
			file >> name >> std::ws;
			
			LoadPlayer(name);
		}
		return true;
	}

	void PlayerDatabase::LoadPlayer(string p_name)
	{
		entityid id = 0;
		string temp;
		p_name = PlayerFileName(p_name);
		ifstream file(p_name.c_str());
		file >> temp >> id;
		m_map[id].ID() = id;
		file >> m_map[id] >> std::ws;
		USERLOG.Log("Loaded Player: " + m_map[id].Name());
	}

	bool PlayerDatabase::Save()
	{
		ofstream file("players/players.txt");
		iterator itr = begin();

		while (itr != end())
		{
			file << itr->Name() << "\n";
			SavePlayer(itr->ID());
			++itr;
			USERLOG.Log("Save player");

		}
		return true;
	}

	void PlayerDatabase::SavePlayer(entityid p_player)
	{
		map<entityid, Player>::iterator itr = m_map.find(p_player);
		if (itr == m_map.end())
			return;
		string name = PlayerFileName(itr->second.Name());
		ofstream file(name.c_str());
		file << "[ID]			" << p_player << "\n";
		file << itr->second;
	}

	bool PlayerDatabase::AddPlayer(Player& p_player)
	{
		if (has(p_player.ID()))
			return false;
		if (hasfull(p_player.Name()))
			return false;
		m_map[p_player.ID()] = p_player;
		ofstream file("players/players.txt", std::ios::app);
		file << p_player.Name() << "\n";
		ERRORLOG.Log("Success");
		SavePlayer(p_player.ID());
		return true;
	}

	PlayerDatabase::iterator PlayerDatabase::findactive(const string& p_name)
	{
		return double_find_if(begin(), end(), matchentityfull(p_name), matchentity(p_name), playeractive());
	}

	PlayerDatabase::iterator PlayerDatabase::findloggedin(const string& p_name)
	{
		return double_find_if(begin(), end(), matchentityfull(p_name), matchentity(p_name), playerloggedin());
	}

	void PlayerDatabase::Logout(entityid p_player)
	{
		Player& p = get(p_player);
		USERLOG.Log(SocketLib::GetIPString(p.Conn()->GetRemoteAddress()) +
			" - User " + p.Name() + " logged off.");
		p.Conn() = nullptr;
		p.LoggedIn() = false;
		p.Active() = false;
		SavePlayer(p_player);
	}
}