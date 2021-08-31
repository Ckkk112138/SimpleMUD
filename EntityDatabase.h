#ifndef ENTITYDATABASE_H
#define ENTITYDATABASE_H

#include <map>
#include "Entity.h"
#include <vector>

namespace SimpleMUD
{
	template<class datatype>
	class EntityDatabase
	{
	public:
		static std::map<entityid, datatype> m_map;
	public:
		class iterator: public std::map<entityid,datatype>::iterator
		{
		public:
			iterator(){}
			iterator(const typename std::map<entityid, datatype>::iterator& p_itr)
				:m_itr(p_itr)
			{}
			datatype& operator*()
			{
				return m_itr->second;
			}
			datatype* operator->()
			{
				return &(m_itr->second);
			}
		private:
			typename std::map<entityid, datatype>::iterator m_itr;
	
		};
		static iterator begin() { return iterator(m_map.begin()); }
		static iterator end() { return iterator(m_map.end()); }
		static bool has(entityid p_id)
		{
			return m_map.find(p_id) != m_map.end();
		}
		static iterator find(entityid p_id)
		{
			return iterator(m_map.find(p_id));
		}
		static datatype& get(entityid p_id)
		{
			return m_map[p_id];
		}
		static iterator find(const std::string& p_name)
		{
			return BasicLib::double_find_if(begin(), end(),
				matchentityfull(p_name), matchentity(p_name));
		}
		static bool has(std::string p_name)
		{
			return find(p_name) != end();
		}
		static iterator findfull(const std::string& p_name)
		{
			return std::find_if(begin(), end(), matchentityfull(p_name));
		}
		static bool hasfull(std::string p_name)
		{
			return findfull(p_name) != end();
		}
		static size_t size()
		{
			return m_map.size();
		}
		
	};

	template<class datatype>
	std::map<entityid, datatype> EntityDatabase<datatype>::m_map;

	template<class datatype>
	class EntityDatabaseVector
	{
	protected:
		static std::vector<datatype> m_vector;
	public:
		typedef typename std::vector<datatype>::iterator iterator;
		static iterator begin() { return m_vector.begin() + 1; }
		static iterator end() { return m_vector.end(); }
		static size_t size() { return m_vector.size() - 1; }
		static datatype& get(entityid p_id)
		{
			if (p_id >= m_vector.size() || p_id == 0)
				throw std::exception();
			return m_vector[p_id];
		}
	};
	template<class datatype>
	std::vector<datatype> EntityDatabaseVector<datatype>::m_vector;
}
#endif // !ENTITYDATABASE_H

