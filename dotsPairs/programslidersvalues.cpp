#include "programslidersvalues.h"

ProgramSlidersValues::ProgramSlidersValues()
{

}

ProgramSlidersValues::ProgramSlidersValues(const ProgramSlidersValues &other)
{
    for (int i = 0; i < 2; i++){
        upDexterValue[i] = other.upDexterValue[i];
        upDexterAntValue[i] = other.upDexterAntValue[i];
        downDexterValue[i] = other.downDexterValue[i];
        downDexterAntValue[i] = other.downDexterAntValue[i];

        upSinisterValue[i] = other.upSinisterValue[i];
        upSinisterAntValue[i] = other.upSinisterAntValue[i];
        downSinisterValue[i] = other.downSinisterValue[i];
        downSinisterAntValue[i] = other.downSinisterAntValue[i];
        neckValue[i] = other.neckValue[i];
    }
}

ProgramSlidersValues &ProgramSlidersValues::operator=(const ProgramSlidersValues &other)
{
    for (int i = 0; i < 2; i++){
        upDexterValue[i] = other.upDexterValue[i];
        upDexterAntValue[i] = other.upDexterAntValue[i];
        downDexterValue[i] = other.downDexterValue[i];
        downDexterAntValue[i] = other.downDexterAntValue[i];

        upSinisterValue[i] = other.upSinisterValue[i];
        upSinisterAntValue[i] = other.upSinisterAntValue[i];
        downSinisterValue[i] = other.downSinisterValue[i];
        downSinisterAntValue[i] = other.downSinisterAntValue[i];
        neckValue[i] = other.neckValue[i];
    }
    return *this;
}

void ProgramSlidersValues::setUpDexterValue(int index, int value)
{
    upDexterValue[index] = value;
}

void ProgramSlidersValues::setUpDexterAntValue(int index, int value)
{
    upDexterAntValue[index] = value;
}

void ProgramSlidersValues::setDownDexterValue(int index, int value)
{
    downDexterValue[index] = value;
}

void ProgramSlidersValues::setDownDexterAntValue(int index, int value)
{
    downDexterAntValue[index] = value;
}

void ProgramSlidersValues::setUpSinisterValue(int index, int value)
{
    upSinisterValue[index] = value;
}

void ProgramSlidersValues::setUpSinisterAntValue(int index, int value)
{
    upSinisterAntValue[index] = value;
}

void ProgramSlidersValues::setDownSinisterValue(int index, int value)
{
    downSinisterValue[index] = value;
}

void ProgramSlidersValues::setDownSinisterAntValue(int index, int value)
{
    downSinisterAntValue[index] = value;
}

void ProgramSlidersValues::setNeckValue(int index, int value)
{
    neckValue[index] = value;
}

int ProgramSlidersValues::getUpDexterValue(int index) const
{
    return upDexterValue[index];
}

int ProgramSlidersValues::getUpDexterAntValue(int index) const
{
    return upDexterAntValue[index];
}

int ProgramSlidersValues::getDownDexterValue(int index) const
{
    return downDexterValue[index];
}

int ProgramSlidersValues::getDownDexterAntValue(int index) const
{
    return downDexterAntValue[index];
}

int ProgramSlidersValues::getUpSinisterValue(int index) const
{
    return upSinisterValue[index];
}

int ProgramSlidersValues::getUpSinisterAntValue(int index) const
{
    return upSinisterAntValue[index];
}

int ProgramSlidersValues::getDownSinisterValue(int index) const
{
    return downSinisterValue[index];
}

int ProgramSlidersValues::getDownSinisterAntValue(int index) const
{
    return downSinisterAntValue[index];
}

int ProgramSlidersValues::getNeckValue(int index) const
{
    return neckValue[index];
}
