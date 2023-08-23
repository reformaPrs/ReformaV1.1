#include "simpledotspair.h"



SimpleDotsPair::SimpleDotsPair(int partID, int id):
    partID(partID),
    id(id)
{
    value = 0;
}

SimpleDotsPair::SimpleDotsPair(SimpleDotsPair *other):
    value(other->value),
    partID(other->partID),
    id(other->id)
{

}

SimpleDotsPair::~SimpleDotsPair()
{

}

int SimpleDotsPair::getValue() const
{
    return value;
}

void SimpleDotsPair::setValue(int newValue)
{
    if(newValue >= 25 || newValue == 0) {
        value = newValue;
    }
}

int SimpleDotsPair::getID() const
{
    return id;
}

int SimpleDotsPair::getPartID() const
{
    return partID;
}

bool SimpleDotsPair::getIsInDexterGroup() const
{
    return (static_cast<int>(type) / 100 == 1);
}

bool SimpleDotsPair::getIsInUpGroup() const
{
    return (static_cast<int>(type) / 10 % 10 == 1);
}

bool SimpleDotsPair::getIsInAntagonistGroup() const
{
    return (static_cast<int>(type) % 10 == 1);
}

const SimpleDotsPair::PairType &SimpleDotsPair::getType() const
{
    return type;
}

void SimpleDotsPair::setType(const PairType &newType)
{
    type = newType;
}
