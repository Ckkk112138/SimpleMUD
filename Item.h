#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <string>
#include "Entity.h"
#include "Attributes.h"

using std::istream;
using std::ostream;

namespace SimpleMUD
{
    typedef unsigned long int money;

    class Item: public Entity
    {
    protected:
        ItemType m_type;
        int m_min;
        int m_max;
        int m_speed;
        money m_price;
        AttributeSet m_attributes;
    public:
        Item()
        {
            m_type = ARMOR;
            m_min = 0;
            m_max = 0;
            m_speed = 0;
            m_price = 0;
        }

        ItemType& Type() { return m_type; }

        int& GetAttr(int p_attr)
        {
            return m_attributes[p_attr];
        }

        int& Min() { return m_min; }
        int& Max() { return m_max; }
        int& Speed() { return m_speed; }
        money& Price() { return m_price; }
        friend istream& operator>>(istream& p_stream, Item& i);

    };

    inline istream& operator>>(istream& p_stream, Item& i) {
        std::string temp;
        // remove white space before getline()
        p_stream >> temp >> std::ws;   
        std::getline(p_stream, i.m_name);

        p_stream >> temp >> temp;      
        i.m_type = GetItemType(temp);

        p_stream >> temp >> i.m_min;
        p_stream >> temp >> i.m_max;
        p_stream >> temp >> i.m_speed;
        p_stream >> temp >> i.m_price;
        p_stream >> i.m_attributes;
        return p_stream;
    }


}
#endif // !ITEM_H

