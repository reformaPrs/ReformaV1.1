#include "intervaldotspair.h"

IntervalDotsPair::IntervalDotsPair(int partID, int id, QString name):
    SimpleDotsPair(partID, id),
    name(name)
{

}

IntervalDotsPair::IntervalDotsPair(const SimpleDotsPair *otherDotsPair, QString name):
    SimpleDotsPair(otherDotsPair->getPartID(), otherDotsPair->getID()),
    name(name)
{
    setValue(otherDotsPair->getValue());
}

IntervalDotsPair::IntervalDotsPair(IntervalDotsPair *other):
    SimpleDotsPair(other->getPartID(), other->getID()),
    startTime(other->startTime),
    endTime(other->endTime),
    name(other->name)
{
    setValue(other->getValue());
}

IntervalDotsPair::~IntervalDotsPair()
{

}

int IntervalDotsPair::getStartTime() const
{
    return startTime;
}

void IntervalDotsPair::setStartTime(int newStartTime)
{
    startTime = newStartTime;
}

int IntervalDotsPair::getEndTime() const
{
    return endTime;
}

void IntervalDotsPair::setEndTime(int newEndTime)
{
    endTime = newEndTime;
}

const QString &IntervalDotsPair::getName() const
{
    return name;
}

QString IntervalDotsPair::toString()
{
    return name + " start " + QString::number(startTime) + " end " + QString::number(endTime)
            + " id " + QString::number(getID()) + " value " + QString::number(getValue());
}

bool IntervalDotsPair::equals(const IntervalDotsPair &other)
{
    return getID() == other.getID() &&
            getValue() == other.getValue() &&
            startTime == other.startTime &&
            endTime == other.endTime;
}
