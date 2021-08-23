#include "DatabasePointer.h"
#include "ItemDatabase.h"

namespace SimpleMUD
{
#define DATABASEPOINTERIMPL(pt,t,db)
	t& pt::operator*()
	{
		return db::get(m_id);
	}

	t* pt::operator->()
	{
		return &(db::get(m_id));
	}

	pt::operator t* ()
	{
		if (m_id == 0)
			return nullptr;
		return &(db::get(m_id));
	}

	DATABASEPOINTERIMPL(player,Player,PlayerDatabase)
}
