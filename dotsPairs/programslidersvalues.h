#ifndef PROGRAMSLIDERSVALUES_H
#define PROGRAMSLIDERSVALUES_H


class ProgramSlidersValues
{
public:
    ProgramSlidersValues();

    ProgramSlidersValues(const ProgramSlidersValues &other);

    ProgramSlidersValues& operator=(const ProgramSlidersValues &other);

    void setUpDexterValue(int index, int value);
    void setUpDexterAntValue(int index, int value);
    void setDownDexterValue(int index, int value);
    void setDownDexterAntValue(int index, int value);
    void setUpSinisterValue(int index, int value);
    void setUpSinisterAntValue(int index, int value);
    void setDownSinisterValue(int index, int value);
    void setDownSinisterAntValue(int index, int value);
    void setNeckValue(int index, int value);

    int getUpDexterValue(int index) const;
    int getUpDexterAntValue(int index) const;
    int getDownDexterValue(int index) const;
    int getDownDexterAntValue(int index) const;
    int getUpSinisterValue(int index) const;
    int getUpSinisterAntValue(int index) const;
    int getDownSinisterValue(int index) const;
    int getDownSinisterAntValue(int index) const;
    int getNeckValue(int index) const;
private:
    int upDexterValue[2] = {0};
    int upDexterAntValue[2] = {0};
    int downDexterValue[2] = {0};
    int downDexterAntValue[2] = {0};
    int upSinisterValue[2] = {0};
    int upSinisterAntValue[2] = {0};
    int downSinisterValue[2] = {0};
    int downSinisterAntValue[2] = {0};
    int neckValue[2] = {0};
};

#endif // PROGRAMSLIDERSVALUES_H
