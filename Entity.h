#ifndef ENTITY_H
#define ENTITY_H

#include "BasicLib.h"
#include <string>

namespace SimpleMUD
{
	typedef unsigned int entityid;
	class Entity
	{
	public:
		Entity() :m_name("UNDEFINED"), m_id(0) {}
		std::string& Name() { return m_name; }
		entityid& ID() { return m_id; }
		std::string CompName() { return BasicLib::LowerCase(m_name); }
		bool FullMatch(const std::string& p_name) { return CompName() == BasicLib::LowerCase(p_name); }
		bool Match(const std::string& p_name)
		{
			if (p_name.size() == 0)
				return true;
			std::string name = CompName();
			std::string search = BasicLib::LowerCase(p_name);
			size_t pos = name.find(search);
			while (pos != std::string::npos)
			{
				if (pos == 0 || m_name[pos - 1] == ' ')
				{
					return true;
				}
				pos = name.find(search, pos + 1);
			}
		}
	protected:
		std::string m_name;
		entityid m_id;
	};

	struct matchentityfull
	{
		std::string m_str;
		matchentityfull(const std::string& p_str)
			:m_str(p_str)
		{

		}
		bool operator()(Entity& p_entity)
		{
			return p_entity.FullMatch(m_str);
		}
		bool operator()(Entity* p_entity)
		{
			return p_entity != nullptr && p_entity->FullMatch(m_str);
		}

	};
	struct matchentity
	{
		std::string m_str;
		matchentity(const std::string& p_str)
			:m_str(p_str)
		{

		}
		bool operator()(Entity& p_entity)
		{
			return p_entity.Match(m_str);
		}
		bool operator()(Entity* p_entity)
		{
			return p_entity != nullptr && p_entity->Match(m_str);
		}

	};
}
#endif // !ENTITY_H

