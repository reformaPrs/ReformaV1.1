#include "abstractbodypart.h"

AbstractBodyPart::AbstractBodyPart(int id, QObject *parent)
    : QObject{parent}, id(id)
{

}

AbstractBodyPart::~AbstractBodyPart()
{

}

int AbstractBodyPart::getId() const
{
    return id;
}

AbstractBodyPart::Status AbstractBodyPart::getStatus() const{
    return status;
}
