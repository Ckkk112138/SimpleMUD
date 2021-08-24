#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include <string>
#include "BasicLib.h"

namespace SimpleMUD
{
    enum Attribute {
        STRENGTH = 0,
        HEALTH = 1,
        AGILITY = 2,
        MAXHITPOINTS = 3,
        ACCURACY = 4,
        DODGING = 5,
        STRIKEDAMAGE = 6,
        DAMAGEABSORB = 7,
        HPREGEN = 8
    };
    const int NUMATTRIBUTES = 9;
    static const std::string ATTRIBUTESTRINGS[NUMATTRIBUTES] = { "STRENGTH",
    "HEALTH","AGILITY","MAXHITPOINTS","ACCURACY","DODGING",
    "STRIKEDAMAGE","DAMAGEABSORB","HPREGEN" };

    inline Attribute GetAttribute(std::string p_attr)
    {
        std::string name = BasicLib::UppercCase(p_attr);
        for (int i = 0; i < NUMATTRIBUTES; ++i)
        {
            if (ATTRIBUTESTRINGS[i] == p_attr)
                return Attribute(i);
        }
        return Attribute(0);
    }

    inline std::string GetAttributeString(Attribute p_attr)
    {
        return ATTRIBUTESTRINGS[p_attr];
    }

    class AttributeSet
    {
    protected:
        int m_attributes[NUMATTRIBUTES];
    public:
        AttributeSet()
        {
            for (int i = 0; i < NUMATTRIBUTES; ++i)
                m_attributes[i] = 0;
        }
        int& operator[](int p_attr)
        {
            return m_attributes[p_attr];
        }
        friend std::ostream& operator<<(std::ostream& p_stream, const AttributeSet& a);
        friend std::istream& operator>>(std::istream& p_stream, AttributeSet& a);
        
    };

    inline std::ostream& operator<<(std::ostream& p_stream, const AttributeSet& a)
    {
        for (int i = 0; i < NUMATTRIBUTES; ++i)
        {
            p_stream << "[" << GetAttributeString((Attribute)i) << "] " << a.m_attributes[i] << "\n";

        }
        return p_stream;
    }

    inline std::istream& operator>>(std::istream& p_stream, AttributeSet& a)
    {
        std::string temp;
        for (int i = 0; i < NUMATTRIBUTES; ++i)
            p_stream >> temp >> a.m_attributes[i];
        return p_stream;
    }

    enum ItemType
    {
        WEAPON,
        ARMOR,
        HEALING
    };

    const int NUMITEMTYPES = 4;

    const std::string ITEMSTRINGS[NUMITEMTYPES] =
    {
        "WEAPON",
        "ARMOR",
        "HEALING",
        "SPECIAL"
    };

    inline ItemType GetItemType(std::string p_itemtype)
    {
        std::string name = BasicLib::UppercCase(p_itemtype);
        for (int i = 0; i < NUMITEMTYPES; ++i)
        {
            if (ITEMSTRINGS[i] == p_itemtype)
                return ItemType(i);
        }
        return ItemType(0);
    }

    inline std::string GetItemTypeString(ItemType p_itemtype)
    {
        return ITEMSTRINGS[p_itemtype];
    }
}
#endif // !ATTRIBUTES_H

