#ifndef ENTITYDATABASE_H
#define ENTITYDATABASE_H

#include <map>
#include "Entity.h"
#include <vector>
#include <iostream>
#include <iterator>



namespace SimpleMUD
{
	template<class datatype>
	class EntityDatabase
	{
	protected:
		static std::map<entityid, datatype> m_map;
	public:
		
		class iterator: public std::map<entityid,datatype>::iterator
		{
		public:
			typedef typename std::map<entityid, datatype>::iterator eitr;
			iterator(){}
			iterator(const typename std::map<entityid, datatype>::iterator& p_itr)
			{
				eitr& itr = *this;
				itr = p_itr;
			}
			datatype& operator*()
			{
				eitr& itr = *this;
				return itr->second;
			}
			datatype* operator->()
			{
				eitr& itr = *this;
				return &(itr->second);
			}
	
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
		static  iterator findfull(const std::string& p_name)
		{
			matchentityfull m(p_name);
			std::cout << p_name << "!!!!" << std::endl;
			
			
			return std::find_if(begin(), end(), m);
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
	
}
#endif // !ENTITYDATABASE_H

