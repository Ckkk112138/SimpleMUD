#include "ItemDatabase.h"

namespace SimpleMUD
{
	std::map<entityid, Item> EntityDatabase<Item>::m_map;

	bool ItemDatabase::Load()
	{
		std::ifstream file("items/items.itm");
		entityid id;
		std::string temp;
		while (!file.eof())
		{
			file >> temp >> id;
			m_map[id].ID() = id;
			file >> m_map[id] >> std::ws;
			USERLOG.Log("Loaded Item: " + m_map[id].Name());
		}
		return true;
	}	
}