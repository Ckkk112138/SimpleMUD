#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "SocketLib.h"
#include "BasicLib.h"
#include "Entity.h"
#include"Attributes.h"
#include "Item.h"


#include "DatabasePointer.h"


using SocketLib::Connection;
using SocketLib::Telnet;
using namespace BasicLib;
using std::ostream;
using std::istream;
using std::string; 

namespace SimpleMUD
{
	
	// temp def
	typedef entityid room;

	const int PLAYERITEMS = 16;
	class Player :public Entity
	{
	protected:
		// info
		string m_password;
		PlayerRank m_rank;
		// attributes
		int m_statpoints;
		int m_experience;
		int m_level;
		room m_room;
		money m_money;
		int m_hp;
		AttributeSet m_baseattributes;
		AttributeSet m_attributes;
		sint64 m_nextattacktime;
		// inventory

		item m_inventory[PLAYERITEMS];

		int m_items;
		int m_weapon;
		int m_armor;
		// no-savavle info
		Connection<Telnet>* m_connection;
		bool m_loggedin;
		bool m_active;
		bool m_newbie;
	public:
		Player();
		// level func
		int NeedforLevel(int p_level)
		{
			return (int)(100 * (pow(1.4, p_level - 1) - 1));

		}
		int NeedForNextLevel();
		bool Train();
		int Level() { return m_level; }
		// attribute func
		void RecalculateStats();
		void AddHP(int p_hp);
		void SetHP(int p_hp);
		int HitPoints() { return m_hp; }
		int GetAttr(int p_attr)
		{
			int val = m_attributes[p_attr] + m_baseattributes[p_attr];
			if (p_attr == STRENGTH || p_attr == HEALTH || p_attr == AGILITY)
				if (val < 1)
					return 1;
			return val;
		}
		int GetBaseAttr(int p_attr) { return m_baseattributes[p_attr]; }
		void SetBaseAttr(int p_attr, int p_val);
		void AddToBaseAttr(int p_attr, int p_val);

		int& StatPoints() { return m_statpoints; }
		int& Experience() { return m_experience; }
		room& CurrentRoom() { return m_room; }
		money& Money() { return m_money; }

		sint64& NextAttackTime() { return m_nextattacktime; }
		// item func
		item GetItem(int p_index) { return m_inventory[p_index]; }
		int Items() { return m_items; }
		const int MaxItems()const { return PLAYERITEMS; }
		item Weapon()
		{
			if (m_weapon == -1)
				return 0;
			else
			{
				return m_inventory[m_weapon];
			}
		}
		item Armor()
		{
			if (m_armor == -1)
				return 0;
			else
			{
				return m_inventory[m_armor];
			}
		}

		void AddBonuses(item p_item);
		void AddDynamicBonuses(item p_item);

		bool PickUpItem(item p_item);
		bool DropItem(int p_index);
		void RemoveWeapon();
		void RemoveArmor();
		void UseWeapon(int p_index);
		void UseArmor(int p_index);
		int GetItemIndex(const string& p_name);
		// non-savable accessors
		string& Password() { return m_password; }
		PlayerRank& Rank() { return m_rank; }
		Connection<Telnet>*& Conn() { return m_connection; }
		bool& LoggedIn() { return m_loggedin; }
		bool& Active() { return m_active; }
		bool& Newbie() { return m_newbie; }
		//commu func
		void SendString(const string& p_string);
		void PrintStasbar(bool p_update = false);
		// in&out stream func
		friend ostream& operator<<(ostream& p_stream, const Player& p);
		friend istream& operator>>(istream& p_stream, Player& p);
	};

	inline ostream& operator<<(ostream& p_stream, const Player& p)
	{
		p_stream << "[NAME] " << p.m_name << "\n";
		p_stream << "[PASS] " << p.m_password << "\n";
		p_stream << "[NEXTATTACKTIME] " << p.m_nextattacktime << "\n";
		p_stream << "[INVENTORY] ";
		for (int i = 0; i < p.MaxItems(); ++i)
		{
			p_stream << p.m_inventory[i].m_id << " ";
		}
		p_stream << "\n";
		p_stream << "[WEAPON] " << p.m_weapon << "\n";
		p_stream << "[WEAPON] " << p.m_armor << "\n";
		return p_stream;
	}

	inline istream& operator>>(istream& p_stream, Player& p)
	{
		string temp;
		p_stream >> temp >> std::ws;
		std::getline(p_stream, p.m_name);
		p_stream >> temp >> p.m_password;
		p_stream >> temp >> temp;
		p.m_rank = GetRank(temp);
		p_stream >> temp >> p.m_statpoints;
		p_stream >> temp >> p.m_experience;
		p_stream >> temp >> p.m_level;
		p_stream >> temp >> p.m_room;
		p_stream >> temp >> p.m_money;
		p_stream >> temp >> p.m_hp;
		p_stream >> temp;
		p_stream >> p.m_nextattacktime;
		p_stream >> p.m_baseattributes;
		p_stream >> temp;
		p.m_items = 0;
		for (int i = 0; i < p.MaxItems(); i++)
		{
			p_stream >> p.m_inventory[i];
			if (p.m_inventory[i] != nullptr)
				p.m_items++;
		}
		p_stream >> temp >> p.m_weapon;
		p_stream >> temp >> p.m_armor;
		p.RecalculateStats();
		return p_stream;

	}

	struct playeractive
	{
		inline bool operator()(Player& p_player)
		{
			return p_player.Active();
		}
		inline bool operator()(Player* p_player)
		{
			return p_player != nullptr && p_player->Active();
		}
	};

	struct playerloggedin
	{
		inline bool operator()(Player& p)
		{
			return p.LoggedIn();
		}
		inline bool operator()(Player* p)
		{
			return p != nullptr && p->LoggedIn();
		}
	};

	struct playersend
	{
		const string& m_msg;
		playersend(const string& p_msg):m_msg(p_msg){}
		void operator()(Player& p)
		{
			p.SendString(m_msg);
		}
		void operator()(Player* p)
		{
			if (p != nullptr)
				p->SendString(m_msg);
		}
	};
}	
#endif // !PLAYER_H

