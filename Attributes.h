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
    };
}
#endif // !ATTRIBUTES_H

