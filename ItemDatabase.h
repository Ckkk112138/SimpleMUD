#ifndef ITEMDATABASE_H
#define ITEMDATABASE_H

#include <string>
#include <map>
#include "EntityDatabase.h"
#include "Item.h"
#include "SimpleMUDLogs.h"


namespace SimpleMUD
{
	class ItemDatabase :public EntityDatabase<Item>
	{
	public:
		static bool Load();
	};
}
#endif // !ITEMDATABASE_H

