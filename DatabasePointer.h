#ifndef DATABASEPOINTER_H
#define DATABASEPOINTER_H

#include "Entity.h"
#include "EntityDatabase.h"
#include <iostream>



namespace SimpleMUD
{
	
#define DATABASEPOINTER(pt,t)
	
	
	class t;
	class pt
	{
	public:
		pt(entityid p_id = 0) :m_id(p_id){}
		pt& operator=(entityid p_id)
		{
			m_id = p_id;
			return *this;
		}
		operator entityid()
		{
			return m_id;
		}
		operator t* ();
		t& operator*();
		t* operator->();
		entityid m_id;
	};

	// useless class to prevent compiler error in VS2019
	class db
	{
	public:
		static t& get(entityid p_id)
		{

		}
	};

	inline std::ostream& operator<<(std::ostream& s, const pt& p)
	{
		s << p.m_id;
		return s;
	}

	inline std::istream& operator>>(std::istream& s, pt& p)
	{
		s >> p.m_id;
		return s;
	}

	DATABASEPOINTER(item,Item)
	
}

#endif // !DATABASEPOINTER_H

