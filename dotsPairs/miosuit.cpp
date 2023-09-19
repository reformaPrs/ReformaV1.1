#include "miosuit.h"
#include "dotsPairs/programslidersvalues.h"

MioSuit *MioSuit::getMioSuitInstance()
{
    static MioSuit *instance;
    if (instance != nullptr){
        return instance;
    }
    instance = new MioSuit();

    return instance;
}

MioSuit::~MioSuit()
{
    for (auto pair : dotsPairs){
        delete pair;
    }
    for (auto pair : popDotsPairs){
        delete pair;
    }
    for (auto heater : heaters) {
        delete heater;
    }
    for (auto intervalVector : intervalDotsPairsUp) {
        for (auto pair : intervalVector) {
            delete pair;
        }
    }
    for (auto intervalVector : intervalDotsPairsDown) {
        for (auto pair : intervalVector) {
            delete pair;
        }
    }
}

void MioSuit::addToScene(QGraphicsScene *scene)
{
    for (auto pair : dotsPairs) {
        pair->addToScene(scene);
    }
    for(auto heater : heaters){
        heater->addToScene(scene);
    }
}

void MioSuit::translate(Language language)
{
    if (language == RU) {
        for (auto dot : popDotsPairs) {
            dot->translate(RU);
        }
        for (auto heater : heaters){
            heater->translate(RU);
        }
    } else if (language == EN) {
        for (auto dot : popDotsPairs) {
            dot->translate(EN);
        }
        for (auto heater : heaters){
            heater->translate(EN);
        }
    } else if (language == UZ) {
        for (auto dot : popDotsPairs) {
            dot->translate(UZ);
        }
        for (auto heater : heaters){
            heater->translate(UZ);
        }
    }
}

void MioSuit::show()
{
    for(auto item : dotsPairs){
        //NOTE Показывает все
//        item->show();
        if (item->getValue() != 0){
            item->show();
        }
    }
}

void MioSuit::hide()
{
    for(auto item : dotsPairs){
        item->hide();
    }
}

void MioSuit::hideHeaters()
{
    for(auto heater : heaters){
        heater->hide();
    }
}

void MioSuit::showHeaters()
{
    for(auto heater : heaters){
        heater->show();
    }
}

void MioSuit::resetDotsPairs(int partID)
{
    for (auto pair : dotsPairs) {
        if (pair->getPartID() == partID) {
            pair->setValue(0);
            pair->hide();
        }
    }
}

void MioSuit::addPopToScene(int partID, QGraphicsScene *scene)
{
    activePopDotsPairs.clear();
    for (auto pair : popDotsPairs) {
        if (pair->getPartID() == partID) {
            pair->setValue(dotsPairs[pair->getID()]->getValue());
            pair->addToScene(scene);
            if (pair->getValue() == 0) {
                pair->hide();
            }
            activePopDotsPairs.append(pair);
        }
    }
}

void MioSuit::removePopFromScene(QGraphicsScene *scene)
{
    for (auto pair : activePopDotsPairs) {
        pair->removeFromScene(scene);
    }
}

void MioSuit::resetPopDotsPairs(){
    for (auto pair : activePopDotsPairs) {
        pair->setValue(0);
        pair->hide();
    }
}

void MioSuit::activateMusclePopDotsPairs(bool isFirstMuscle)
{
    for (auto pair : activePopDotsPairs) {
        if (pair->getIsFirstMuscle() == isFirstMuscle){
            pair->show();
        }
    }
}

void MioSuit::deactivateMusclePopDotsPairs(bool isFirstMuscle)
{
    for (auto pair : activePopDotsPairs) {
        if (pair->getIsFirstMuscle() == isFirstMuscle){
            pair->setValue(0);
            pair->hide();
        }
    }
}

void MioSuit::showPopDotsPairs()
{
    for (auto pair : activePopDotsPairs) {
        pair->show();
    }
}

bool MioSuit::isMuscleActive(bool isFirstMuscle){
    for (auto pair : activePopDotsPairs) {
        if (pair->getIsFirstMuscle() == isFirstMuscle && pair->getValue() != 0){
            return true;
        }
    }
    return false;
}

bool MioSuit::isPopValuesChanged()
{
    for (auto pair : activePopDotsPairs) {
        if (pair->getValue() != dotsPairs[pair->getID()]->getValue()){
            return true;
        }
    }
    return false;
}

void MioSuit::mergeChanges()
{
    for (auto pair : activePopDotsPairs) {
        int id = pair->getID(), value = pair->getValue();
        dotsPairs[id]->setValue(value);
        if (value != 0) {
            dotsPairs[id]->show();
        } else {
            dotsPairs[id]->hide();
        }
        if (isHalfhourActive){
            initialPairsValues[halfhourIndex][id] = value;
        } else {
            initialPairsValues[0][id] = value;
            initialPairsValues[1][id] = value;
        }
    }
    for (int i = 0; i < 2; i++){
        maxValues.setDownDexterValue(i, 0);
        maxValues.setDownDexterAntValue(i, 0);
        maxValues.setUpDexterValue(i, 0);
        maxValues.setUpDexterAntValue(i, 0);
        maxValues.setDownSinisterValue(i, 0);
        maxValues.setDownSinisterAntValue(i, 0);
        maxValues.setUpSinisterValue(i, 0);
        maxValues.setUpSinisterAntValue(i, 0);
        maxValues.setNeckValue(i, 0);
    }
    for (int i = 0; i < 2; i++){
        for (int pairID = 0; pairID < dotsPairs.size(); pairID++){
            setMaxValue(pairID, i, initialPairsValues[i][pairID]);
        }
    }
    saveHalfhour();
}

int MioSuit::addIntervalPairs(bool isUp)
{
    int count = 0;
    for(auto pair : activePopDotsPairs) {
        if (pair->getValue() != 0) {
            count++;
            IntervalDotsPair *intervalDotPair = new IntervalDotsPair(pair, pairsNames[pair->getID()]);
            intervalDotPair->setStartTime(0);
            intervalDotPair->setEndTime(20);
            if (isUp) {
                intervalDotsPairsUp[halfhourIndex].append(intervalDotPair);
            } else {
                intervalDotsPairsDown[halfhourIndex].append(intervalDotPair);
            }
        }
    }
    return count;
}

IntervalDotsPair *MioSuit::getIntervalDotsPairAt(int index, bool isUp)
{
    if (isUp) {
        return intervalDotsPairsUp[halfhourIndex][index];
    }
    return intervalDotsPairsDown[halfhourIndex][index];
}

void MioSuit::removeIntervalDotsPairAt(int index, bool isUp)
{
    if (isUp) {
        intervalDotsPairsUp[halfhourIndex].remove(index);
        return;
    }
    intervalDotsPairsDown[halfhourIndex].remove(index);
}

bool MioSuit::isIntervalDotsPairWithPartID(int partID, bool isUp)
{
    if (isUp) {
        for (auto pair : intervalDotsPairsUp[halfhourIndex]){
            if (pair->getPartID() == partID){
                return true;
            }
        }
        return false;
    }
    for (auto pair : intervalDotsPairsDown[halfhourIndex]){
        if (pair->getPartID() == partID){
            return true;
        }
    }
    return false;
}

void MioSuit::setDotsPairSimulationValue(int id, int value)
{
    dotsPairs[id]->setValue(value);
    dotsPairs[id]->show();
}

bool MioSuit::isFirstMuscleByID(int id)
{
    return dotsPairs[id]->getIsFirstMuscle();
}

void MioSuit::setRecievedValues(int recievedValues[40][40])
{
    for (int j = 0; j < 2; j++){
        for (auto pair : intervalDotsPairsUp[j]){
            delete pair;
        }
        intervalDotsPairsUp[j].clear();
        for (auto pair : intervalDotsPairsDown[j]){
            delete pair;
        }
        intervalDotsPairsDown[j].clear();

        for (int pairIndex = 0; pairIndex < 40; pairIndex++){
            QVector<int> timings;
            timings.append(0);
            int firstValue = recievedValues[pairIndex][j * MAX_INTERVAL];
            for (int i = j * MAX_INTERVAL; i < MAX_INTERVAL + j * MAX_INTERVAL; i++) {
                if(firstValue != recievedValues[pairIndex][i]){
                    firstValue = recievedValues[pairIndex][i];
                    timings.append(i - j * MAX_INTERVAL);
                }
            }
            timings.append(MAX_INTERVAL);

            if(timings.size() == 2){ // Если не интервальная
                pairsValues[j][pairIndex] = (recievedValues[pairIndex][j * MAX_INTERVAL]);
            } else { //Интервальная
                pairsValues[j][pairIndex] = 0;
                for (qsizetype i = 0; i < timings.size() - 1; i++) {
                    if (recievedValues[pairIndex][timings[i] + j * MAX_INTERVAL] != 0) {
                        IntervalDotsPair *intervalDotsPair = new IntervalDotsPair(dotsPairs[pairIndex], pairsNames[pairIndex]);
                        intervalDotsPair->setValue(recievedValues[pairIndex][timings[i] + j * MAX_INTERVAL]);
                        intervalDotsPair->setStartTime(timings[i]);
                        intervalDotsPair->setEndTime(timings[i+1]);
                        if (pairIndex < 24) {
                            intervalDotsPairsUp[j].append(intervalDotsPair);
                        } else {
                            intervalDotsPairsDown[j].append(intervalDotsPair);
                        }
                    }
                }
            }
        }
    }
    checkForActiveHalfhour();
}

const QVector<IntervalDotsPair *> &MioSuit::getIntervalDotsPairs(bool isUp) const
{
    if (isUp) {
        return intervalDotsPairsUp[halfhourIndex];
    }
    return intervalDotsPairsDown[halfhourIndex];
}

MioSuit::MioSuit(QObject *parent)
    : QObject{parent}
{
    readDotsPairsFromCSV();
    readHeatersFromCSV();
}

void MioSuit::setIsAdminModeActive(bool newIsAdminModeActive)
{
    isAdminModeActive = newIsAdminModeActive;
    for (auto heater: heaters){
        heater->setIsAdminModeActive(newIsAdminModeActive);
    }
    if (isAdminModeActive){
        showInitialValues();
        return;
    }
    showSlidersProgramedValues();
}

const ProgramSlidersValues &MioSuit::getMaxValues() const
{
    return maxValues;
}

bool MioSuit::getIsHalfhourActive() const
{
    return isHalfhourActive;
}

const QVector<DotsPair *> &MioSuit::getDotsPairs() const
{
    return dotsPairs;
}

void MioSuit::setHalfhourIndex(int newHalfhourIndex)
{
    if (newHalfhourIndex != 0 &&
            newHalfhourIndex != 1){
        throw HalfHourException();
    }
    halfhourIndex = newHalfhourIndex;
}

void MioSuit::saveHalfhour()
{
    if (isHalfhourActive) {
        for (qsizetype i = 0; i < dotsPairs.size(); i++) {
            pairsValues[halfhourIndex][i] = dotsPairs[i]->getValue();
        }
        return;
    }
    for (qsizetype i = 0; i < dotsPairs.size(); i++) {
        pairsValues[0][i] = dotsPairs[i]->getValue();
        pairsValues[1][i] = dotsPairs[i]->getValue();
    }
}

void MioSuit::copyHalfhour()
{
    saveHalfhour();
    int otherIndex = 0;
    if (otherIndex == halfhourIndex){
        otherIndex = 1;
    }
    for (qsizetype i = 0; i < pairsValues[0].size(); i++){
        pairsValues[otherIndex][i] = pairsValues[halfhourIndex][i];
    }

    for (auto pair : intervalDotsPairsUp[otherIndex]){
        delete pair;
    }
    intervalDotsPairsUp[otherIndex].clear();
    for (auto pair : intervalDotsPairsUp[halfhourIndex]){
        intervalDotsPairsUp[otherIndex].append(new IntervalDotsPair(pair));
    }

    for (auto pair : intervalDotsPairsDown[otherIndex]){
        delete pair;
    }
    intervalDotsPairsDown[otherIndex].clear();
    for (auto pair : intervalDotsPairsDown[halfhourIndex]){
        intervalDotsPairsDown[otherIndex].append(new IntervalDotsPair(pair));
    }
}

void MioSuit::clearHalfhour()
{
    for (auto pair : dotsPairs) {
        pair->setValue(0);
        pair->hide();
    }
    saveHalfhour();
    if (isHalfhourActive){
        for (auto pair : intervalDotsPairsUp[halfhourIndex]){
            delete pair;
        }
        intervalDotsPairsUp[halfhourIndex].clear();
        for (auto pair : intervalDotsPairsDown[halfhourIndex]){
            delete pair;
        }
        intervalDotsPairsDown[halfhourIndex].clear();
        return;
    }
    for (int j = 0; j < 2; j++){
        for (auto pair : intervalDotsPairsUp[j]){
            delete pair;
        }
        intervalDotsPairsUp[j].clear();
        for (auto pair : intervalDotsPairsDown[j]){
            delete pair;
        }
        intervalDotsPairsDown[j].clear();
    }
}

void MioSuit::update()
{
    for (qsizetype i = 0; i < dotsPairs.size(); i++) {
        int value = pairsValues[halfhourIndex][i];
        dotsPairs[i]->setValue(value);
        if (value == 0){
            dotsPairs[i]->hide();
        } else {
            dotsPairs[i]->show();
        }
    }
}

void MioSuit::saveToFile(QDataStream *out)
{
    for (auto value : pairsValues[0]){
        *out << value;
    }
    if (isHalfhourActive) {
        for (auto value : pairsValues[1]){
            *out << value;
        }
    } else {
        for (auto value : pairsValues[0]){
            *out << value;
        }
    }
    for (auto heater : heaters){
        *out << heater->getIsActive();
    }

    if (isHalfhourActive) {
        for (int i = 0; i < 2; i++) {
            *out << intervalDotsPairsUp[i].size();
            for (auto pair : intervalDotsPairsUp[i]){
                *out << pair->getID() <<
                        pair->getStartTime() <<
                        pair->getEndTime() <<
                        pair->getValue() <<
                        pair->getPartID() <<
                        pair->getName();
            }
            *out << intervalDotsPairsDown[i].size();
            for (auto pair : intervalDotsPairsDown[i]){
                *out << pair->getID() <<
                        pair->getStartTime() <<
                        pair->getEndTime() <<
                        pair->getValue() <<
                        pair->getPartID() <<
                        pair->getName();
            }
        }
    } else {
        for (int i = 0; i < 2; i++) {
            *out << intervalDotsPairsUp[0].size();
            for (auto pair : intervalDotsPairsUp[0]){
                *out << pair->getID() <<
                        pair->getStartTime() <<
                        pair->getEndTime() <<
                        pair->getValue() <<
                        pair->getPartID() <<
                        pair->getName();
            }
            *out << intervalDotsPairsDown[0].size();
            for (auto pair : intervalDotsPairsDown[0]){
                *out << pair->getID() <<
                        pair->getStartTime() <<
                        pair->getEndTime() <<
                        pair->getValue() <<
                        pair->getPartID() <<
                        pair->getName();
            }
        }
    }
}

void MioSuit::readFromFile(QDataStream *in)
{
    for (int i = 0; i < 2; i++){
        maxValues.setDownDexterValue(i, 0);
        maxValues.setDownDexterAntValue(i, 0);
        maxValues.setUpDexterValue(i, 0);
        maxValues.setUpDexterAntValue(i, 0);
        maxValues.setDownSinisterValue(i, 0);
        maxValues.setDownSinisterAntValue(i, 0);
        maxValues.setUpSinisterValue(i, 0);
        maxValues.setUpSinisterAntValue(i, 0);
        maxValues.setNeckValue(i, 0);
    }

    for (qsizetype i = 0; i < pairsValues[0].size(); i++){
        *in >> pairsValues[0][i];
        initialPairsValues[0][i] = pairsValues[0][i];
        setMaxValue(i, 0, pairsValues[0][i]);
    }
    for (qsizetype i = 0; i < pairsValues[1].size(); i++){
        *in >> pairsValues[1][i];
        initialPairsValues[1][i] = pairsValues[1][i];
        setMaxValue(i, 1, pairsValues[1][i]);
    }

    for (auto heater : heaters) {
        bool isActive;
        *in >> isActive;
        heater->setIsActive(isActive);
    }


    for (int i = 0; i < 2; i++) {
        for (auto pair : intervalDotsPairsUp[i]) {
            delete pair;
        }
        intervalDotsPairsUp[i].clear();
        for (auto pair : intervalDotsPairsDown[i]) {
            delete pair;
        }
        intervalDotsPairsDown[i].clear();

        qsizetype n;
        int id, start, end, value, partID;
        QString name;

        *in >> n;
        for (int j = 0; j < n; j++) {
            *in >> id >> start >> end >> value >> partID >> name;
            IntervalDotsPair *pair = new IntervalDotsPair(partID, id, name);
            pair->setStartTime(start);
            pair->setEndTime(end);
            pair->setValue(value);
            intervalDotsPairsUp[i].append(pair);
        }

        *in >> n;
        for (int j = 0; j < n; j++) {
            *in >> id >> start >> end >> value >> partID >> name;
            IntervalDotsPair *pair = new IntervalDotsPair(partID, id, name);
            pair->setStartTime(start);
            pair->setEndTime(end);
            pair->setValue(value);
            intervalDotsPairsDown[i].append(pair);
        }
    }

    checkForActiveHalfhour();
    programValues = maxValues;
}

void MioSuit::saveInitValuesToFile(QDataStream *out)
{
    for (auto value : initialPairsValues[0]){
        *out << value;
    }
    if (isHalfhourActive) {
        for (auto value : initialPairsValues[1]){
            *out << value;
        }
    } else {
        for (auto value : initialPairsValues[0]){
            *out << value;
        }
    }
    for (auto heater : heaters){
        *out << heater->getIsActive();
    }
}

void MioSuit::readInitValuesFromFile(QDataStream *in)
{
    for (int i = 0; i < 2; i++){
        maxValues.setDownDexterValue(i, 0);
        maxValues.setDownDexterAntValue(i, 0);
        maxValues.setUpDexterValue(i, 0);
        maxValues.setUpDexterAntValue(i, 0);
        maxValues.setDownSinisterValue(i, 0);
        maxValues.setDownSinisterAntValue(i, 0);
        maxValues.setUpSinisterValue(i, 0);
        maxValues.setUpSinisterAntValue(i, 0);
        maxValues.setNeckValue(i, 0);
    }

    for (qsizetype i = 0; i < pairsValues[0].size(); i++){
        *in >> pairsValues[0][i];
        initialPairsValues[0][i] = pairsValues[0][i];
        setMaxValue(i, 0, pairsValues[0][i]);
    }
    for (qsizetype i = 0; i < pairsValues[1].size(); i++){
        *in >> pairsValues[1][i];
        initialPairsValues[1][i] = pairsValues[1][i];
        setMaxValue(i, 1, pairsValues[1][i]);
    }

    for (auto heater : heaters) {
        bool isActive;
        *in >> isActive;
        heater->setIsActive(isActive);
    }
    for (int i = 0; i < 2; i++) {
        for (auto pair : intervalDotsPairsUp[i]) {
            delete pair;
        }
        intervalDotsPairsUp[i].clear();
        for (auto pair : intervalDotsPairsDown[i]) {
            delete pair;
        }
        intervalDotsPairsDown[i].clear();
    }
    checkForActiveHalfhour();
    programValues = maxValues;
}

void MioSuit::clear()
{
    for (int i = 0; i < 2; i++){
        maxValues.setDownDexterValue(i, 0);
        maxValues.setDownDexterAntValue(i, 0);
        maxValues.setUpDexterValue(i, 0);
        maxValues.setUpDexterAntValue(i, 0);
        maxValues.setDownSinisterValue(i, 0);
        maxValues.setDownSinisterAntValue(i, 0);
        maxValues.setUpSinisterValue(i, 0);
        maxValues.setUpSinisterAntValue(i, 0);
        maxValues.setNeckValue(i, 0);
    }

    for (qsizetype i = 0; i < pairsValues[0].size(); i++){
        pairsValues[0][i] = 0;
        initialPairsValues[0][i] = pairsValues[0][i];
        setMaxValue(i, 0, pairsValues[0][i]);
    }
    for (qsizetype i = 0; i < pairsValues[1].size(); i++){
        pairsValues[1][i] = 0;
        initialPairsValues[1][i] = pairsValues[1][i];
        setMaxValue(i, 1, pairsValues[1][i]);
    }

    for (auto heater : heaters) {
        heater->setIsActive(false);
    }
    for (int i = 0; i < 2; i++) {
        for (auto pair : intervalDotsPairsUp[i]) {
            delete pair;
        }
        intervalDotsPairsUp[i].clear();
        for (auto pair : intervalDotsPairsDown[i]) {
            delete pair;
        }
        intervalDotsPairsDown[i].clear();
    }
    checkForActiveHalfhour();
    programValues = maxValues;
}

void MioSuit::setMaxValue(int pairIndex, int halfhourIndex, int value){
    switch (dotsPairs[pairIndex]->getType()){
    case SimpleDotsPair::DEXTER_UP_NORMAL:
        if (value > maxValues.getUpDexterValue(halfhourIndex)){
            maxValues.setUpDexterValue(halfhourIndex, value);
        }
        break;
    case SimpleDotsPair::SINISTER_DOWN_NORMAL:
        if (value > maxValues.getDownSinisterValue(halfhourIndex)){
            maxValues.setDownSinisterValue(halfhourIndex, value);
        }
        break;
    case SimpleDotsPair::SINISTER_DOWN_ANTAGONIST:
        if (value > maxValues.getDownSinisterAntValue(halfhourIndex)){
            maxValues.setDownSinisterAntValue(halfhourIndex, value);
        }
        break;
    case SimpleDotsPair::SINISTER_UP_NORMAL:
        if (value > maxValues.getUpSinisterValue(halfhourIndex)){
            maxValues.setUpSinisterValue(halfhourIndex, value);
        }
        break;
    case SimpleDotsPair::SINISTER_UP_ANTAGONIST:
        if (value > maxValues.getUpSinisterAntValue(halfhourIndex)){
            maxValues.setUpSinisterAntValue(halfhourIndex, value);
        }
        break;
    case SimpleDotsPair::DEXTER_DOWN_NORMAL:
        if (value > maxValues.getDownDexterValue(halfhourIndex)){
            maxValues.setDownDexterValue(halfhourIndex, value);

        }
        break;
    case SimpleDotsPair::DEXTER_DOWN_ANTAGONIST:
        if (value > maxValues.getDownDexterAntValue(halfhourIndex)){
            maxValues.setDownDexterAntValue(halfhourIndex, value);
        }
        break;
    case SimpleDotsPair::DEXTER_UP_ANTAGONIST:
        if (value > maxValues.getUpDexterAntValue(halfhourIndex)){
            maxValues.setUpDexterAntValue(halfhourIndex, value);
        }
        break;
    case SimpleDotsPair::NECK:
        if (value > maxValues.getNeckValue(halfhourIndex)){
            maxValues.setNeckValue(halfhourIndex, value);
        }
        break;
    }
}

void MioSuit::showInitialValues()
{
    for (int i = 0; i < 2; i++){
        for (auto dot : dotsPairs){
            int id = dot->getID();
            pairsValues[i][id] = initialPairsValues[i][id];
            if (!isHalfhourActive || i == halfhourIndex){
                dot->setValue(initialPairsValues[i][id]);
                if (initialPairsValues[i][id] > 0){
                    dot->show();
                } else {
                    dot->hide();
                }
            }
        }
    }
}

void MioSuit::showSlidersProgramedValues()
{
    for (int i = 0; i < 2; i++){
        for (auto dot : dotsPairs){
            int id = dot->getID();
            if (initialPairsValues[i][id] != 0) {
                int newValue = 0;
                switch (dot->getType()){
                case SimpleDotsPair::DEXTER_UP_NORMAL:
                    newValue = initialPairsValues[i][id] + programValues.getUpDexterValue(i) - maxValues.getUpDexterValue(i);
                    break;
                case SimpleDotsPair::SINISTER_DOWN_NORMAL:
                    newValue = initialPairsValues[i][id] + programValues.getDownSinisterValue(i) - maxValues.getDownSinisterValue(i);
                    break;
                case SimpleDotsPair::SINISTER_DOWN_ANTAGONIST:
                    newValue = initialPairsValues[i][id] + programValues.getDownSinisterAntValue(i) - maxValues.getDownSinisterAntValue(i);
                    break;
                case SimpleDotsPair::SINISTER_UP_NORMAL:
                    newValue = initialPairsValues[i][id] + programValues.getUpSinisterValue(i) - maxValues.getUpSinisterValue(i);
                    break;
                case SimpleDotsPair::SINISTER_UP_ANTAGONIST:
                    newValue = initialPairsValues[i][id] + programValues.getUpSinisterAntValue(i) - maxValues.getUpSinisterAntValue(i);
                    break;
                case SimpleDotsPair::DEXTER_DOWN_NORMAL:
                    newValue = initialPairsValues[i][id] + programValues.getDownDexterValue(i) - maxValues.getDownDexterValue(i);
                    break;
                case SimpleDotsPair::DEXTER_DOWN_ANTAGONIST:
                    newValue = initialPairsValues[i][id] + programValues.getDownDexterAntValue(i) - maxValues.getDownDexterAntValue(i);
                    break;
                case SimpleDotsPair::DEXTER_UP_ANTAGONIST:
                    newValue = initialPairsValues[i][id] + programValues.getUpDexterAntValue(i) - maxValues.getUpDexterAntValue(i);
                    break;
                case SimpleDotsPair::NECK:
                    newValue = initialPairsValues[i][id] + programValues.getNeckValue(i) - maxValues.getNeckValue(i);
                    break;
                }

                if (newValue <= 0){
                    newValue = 0;
                    dot->hide();
                } else {
                    dot->show();
                }
                pairsValues[i][id] = newValue;
                if (!isHalfhourActive || i == halfhourIndex){
                    dot->setValue(newValue);
                }
            }
        }
    }
}

void MioSuit::readFromFile(QByteArray &data)
{
    for (int i = 0; i < 2; i++){
        maxValues.setDownDexterValue(i, 0);
        maxValues.setDownDexterAntValue(i, 0);
        maxValues.setUpDexterValue(i, 0);
        maxValues.setUpDexterAntValue(i, 0);
        maxValues.setDownSinisterValue(i, 0);
        maxValues.setDownSinisterAntValue(i, 0);
        maxValues.setUpSinisterValue(i, 0);
        maxValues.setUpSinisterAntValue(i, 0);
        maxValues.setNeckValue(i, 0);
    }
    halfhourIndex = 0;
    isHalfhourActive = false;
    for (int i = 0; i < 2; i++) {
        for (auto pair : intervalDotsPairsUp[i]) {
            delete pair;
        }
        intervalDotsPairsUp[i].clear();
        for (auto pair : intervalDotsPairsDown[i]) {
            delete pair;
        }
        intervalDotsPairsDown[i].clear();
    }

    int dataIndex = 0;
    int pairIndex = 0;
    for (int k = 0; k < 4; k++){
        dataIndex += 3;
        while (data[dataIndex] != '/'){
            int letters[] = {data[dataIndex], data[dataIndex + 1]};
            for (int i = 0; i < 2; i++){
                if (letters[i] <= '9'){
                    letters[i] -= '0';
                } else {
                    letters[i] -= 0x37;
                }
            }
            int value = (letters[0] << 4) | letters[1];

            if (value != 0) {
                value = (value & ~0x80) * 5 + 20;
            }
//            qDebug() << data[dataIndex] << " " << data[dataIndex + 1];
//            qDebug() << letters[0] << " " << letters[1];
//            qDebug() << QString::number(letters[0], 2) << " " << QString::number(letters[1], 2);
//            qDebug() << pairIndex << " " << value;

            pairsValues[0][pairIndex] = value;
            pairsValues[1][pairIndex] = value;
            initialPairsValues[0][pairIndex] = value;
            initialPairsValues[1][pairIndex] = value;

            setMaxValue(pairIndex, 0, value);
            setMaxValue(pairIndex, 1, value);

            pairIndex++;
            dataIndex += 2;
        }
        dataIndex += 3;
    }
    programValues = maxValues;
}

void MioSuit::checkForActiveHalfhour(){
    bool isValuesEqual = true;
    for (qsizetype i = 0; i < pairsValues[1].size(); i++) {
        if (pairsValues[0][i] != pairsValues[1][i]) {
            isValuesEqual = false;
            break;
        }
    }
    bool isIntervalValuesEqual = true;
    if (intervalDotsPairsUp[0].size() != intervalDotsPairsUp[1].size() ||
            intervalDotsPairsDown[0].size() != intervalDotsPairsDown[1].size()){
        isIntervalValuesEqual = false;
    } else {
        for (qsizetype i = 0; i < intervalDotsPairsUp[0].size(); i++){
            if (!intervalDotsPairsUp[0][i]->equals(intervalDotsPairsUp[1][i])){
                isIntervalValuesEqual = false;
                break;
            }
        }
        if (isIntervalValuesEqual) {
            for (qsizetype i = 0; i < intervalDotsPairsDown[0].size(); i++){
                if (!intervalDotsPairsDown[0][i]->equals(intervalDotsPairsDown[1][i])){
                    isIntervalValuesEqual = false;
                    break;
                }
            }
        }
    }

    if (isValuesEqual && isIntervalValuesEqual) {
        halfhourIndex = 0;
        isHalfhourActive = false;
    } else {
        isHalfhourActive = true;
    }
}

void MioSuit::activateHalfhour()
{
    isHalfhourActive = true;
    for (auto pair : intervalDotsPairsUp[1]) {
        delete  pair;
    }
    intervalDotsPairsUp[1].clear();
    for (auto pair : intervalDotsPairsDown[1]) {
        delete  pair;
    }
    intervalDotsPairsDown[1].clear();
    for (qsizetype i = 0; i < intervalDotsPairsUp[0].size(); i++){
        intervalDotsPairsUp[1].append(new IntervalDotsPair(intervalDotsPairsUp[0][i]));
    }
    for (qsizetype i = 0; i < intervalDotsPairsDown[0].size(); i++){
        intervalDotsPairsDown[1].append(new IntervalDotsPair(intervalDotsPairsUp[0][i]));
    }
}

void MioSuit::deactivateHalfhour()
{
    isHalfhourActive = false;
    for (auto pair : intervalDotsPairsUp[1]) {
        delete  pair;
    }
    intervalDotsPairsUp[1].clear();
    for (auto pair : intervalDotsPairsDown[1]) {
        delete  pair;
    }
    intervalDotsPairsDown[1].clear();
}

void MioSuit::setProgramSlidersValues(const ProgramSlidersValues values){
    programValues = values;

//    if (isAdminModeActive){
//        showInitialValues();
//        return;
//    }
    showSlidersProgramedValues();
}

ProgramSlidersValues MioSuit::getMinSliderValues()
{
    ProgramSlidersValues minSliderValues;
    for (int i = 0; i < 2; i++){
        constexpr int kInitValue = 25;
        minSliderValues.setUpDexterValue(i, kInitValue);
        minSliderValues.setUpDexterAntValue(i, kInitValue);
        minSliderValues.setDownDexterValue(i, kInitValue);
        minSliderValues.setDownDexterAntValue(i, kInitValue);

        minSliderValues.setUpSinisterValue(i, kInitValue);
        minSliderValues.setUpSinisterAntValue(i, kInitValue);
        minSliderValues.setDownSinisterValue(i, kInitValue);
        minSliderValues.setDownSinisterAntValue(i, kInitValue);
        minSliderValues.setNeckValue(i, kInitValue);
    }

    for (int i = 0; i < 2; i++){
        for (auto dot : dotsPairs){
            int id = dot->getID();
            if (initialPairsValues[i][id] != 0) {
                int minValue, maxValue;
                switch (dot->getType()){
                case SimpleDotsPair::DEXTER_UP_NORMAL:
                    minValue = minSliderValues.getUpDexterValue(i);
                    maxValue = maxValues.getUpDexterValue(i);
                    break;
                case SimpleDotsPair::SINISTER_DOWN_NORMAL:
                    minValue = minSliderValues.getDownSinisterValue(i);
                    maxValue = maxValues.getDownSinisterValue(i);
                    break;
                case SimpleDotsPair::SINISTER_DOWN_ANTAGONIST:
                    minValue = minSliderValues.getDownSinisterAntValue(i);
                    maxValue = maxValues.getDownSinisterAntValue(i);
                    break;
                case SimpleDotsPair::SINISTER_UP_NORMAL:
                    minValue = minSliderValues.getUpSinisterValue(i);
                    maxValue = maxValues.getUpSinisterValue(i);
                    break;
                case SimpleDotsPair::SINISTER_UP_ANTAGONIST:
                    minValue = minSliderValues.getUpSinisterAntValue(i);
                    maxValue = maxValues.getUpSinisterAntValue(i);
                    break;
                case SimpleDotsPair::DEXTER_DOWN_NORMAL:
                    minValue = minSliderValues.getDownDexterValue(i);
                    maxValue = maxValues.getDownDexterValue(i);
                    break;
                case SimpleDotsPair::DEXTER_DOWN_ANTAGONIST:
                    minValue = minSliderValues.getDownDexterAntValue(i);
                    maxValue = maxValues.getDownDexterAntValue(i);
                    break;
                case SimpleDotsPair::DEXTER_UP_ANTAGONIST:
                    minValue = minSliderValues.getUpDexterAntValue(i);
                    maxValue = maxValues.getUpDexterAntValue(i);
                    break;
                case SimpleDotsPair::NECK:
                    minValue = minSliderValues.getNeckValue(i);
                    maxValue = maxValues.getNeckValue(i);
                    break;
                }

                if (minValue < 25 - initialPairsValues[i][id] + maxValue){
                    minValue = 25 - initialPairsValues[i][id] + maxValue;
                    switch (dot->getType()){
                    case SimpleDotsPair::DEXTER_UP_NORMAL:
                        minSliderValues.setUpDexterValue(i, minValue);
                        break;
                    case SimpleDotsPair::SINISTER_DOWN_NORMAL:
                        minSliderValues.setDownSinisterValue(i, minValue);
                        break;
                    case SimpleDotsPair::SINISTER_DOWN_ANTAGONIST:
                        minSliderValues.setDownSinisterAntValue(i, minValue);
                        break;
                    case SimpleDotsPair::SINISTER_UP_NORMAL:
                        minSliderValues.setUpSinisterValue(i, minValue);
                        break;
                    case SimpleDotsPair::SINISTER_UP_ANTAGONIST:
                        minSliderValues.setUpSinisterAntValue(i, minValue);
                        break;
                    case SimpleDotsPair::DEXTER_DOWN_NORMAL:
                        minSliderValues.setDownDexterValue(i, minValue);
                        break;
                    case SimpleDotsPair::DEXTER_DOWN_ANTAGONIST:
                        minSliderValues.setDownDexterAntValue(i, minValue);
                        break;
                    case SimpleDotsPair::DEXTER_UP_ANTAGONIST:
                        minSliderValues.setUpDexterAntValue(i, minValue);
                        break;
                    case SimpleDotsPair::NECK:
                        minSliderValues.setNeckValue(i, minValue);
                        break;
                    }
                }
            }
        }
    }

    return minSliderValues;
}

ProgramSlidersValues MioSuit::getMaxSliderValues()
{
    ProgramSlidersValues maxSliderValues;
    for (int i = 0; i < 2; i++){
        constexpr int kInitValue = 170;
        maxSliderValues.setUpDexterValue(i, kInitValue);
        maxSliderValues.setUpDexterAntValue(i, kInitValue);
        maxSliderValues.setDownDexterValue(i, kInitValue);
        maxSliderValues.setDownDexterAntValue(i, kInitValue);

        maxSliderValues.setUpSinisterValue(i, kInitValue);
        maxSliderValues.setUpSinisterAntValue(i, kInitValue);
        maxSliderValues.setDownSinisterValue(i, kInitValue);
        maxSliderValues.setDownSinisterAntValue(i, kInitValue);
        maxSliderValues.setNeckValue(i, kInitValue);
    }

    return maxSliderValues;

    //Этот код теперь не нужен, но осталю на всякий случай
    for (int i = 0; i < 2; i++){
        for (auto dot : dotsPairs){
            int id = dot->getID();
            if (initialPairsValues[i][id] != 0) {
                int maxValue;
                switch (dot->getType()){
                case SimpleDotsPair::DEXTER_UP_NORMAL:
                    maxValue = maxSliderValues.getUpDexterValue(i);
                    break;
                case SimpleDotsPair::SINISTER_DOWN_NORMAL:
                    maxValue = maxSliderValues.getDownSinisterValue(i);
                    break;
                case SimpleDotsPair::SINISTER_DOWN_ANTAGONIST:
                    maxValue = maxSliderValues.getDownSinisterAntValue(i);
                    break;
                case SimpleDotsPair::SINISTER_UP_NORMAL:
                    maxValue = maxSliderValues.getUpSinisterValue(i);
                    break;
                case SimpleDotsPair::SINISTER_UP_ANTAGONIST:
                    maxValue = maxSliderValues.getUpSinisterAntValue(i);
                    break;
                case SimpleDotsPair::DEXTER_DOWN_NORMAL:
                    maxValue = maxSliderValues.getDownDexterValue(i);
                    break;
                case SimpleDotsPair::DEXTER_DOWN_ANTAGONIST:
                    maxValue = maxSliderValues.getDownDexterAntValue(i);
                    break;
                case SimpleDotsPair::DEXTER_UP_ANTAGONIST:
                    maxValue = maxSliderValues.getUpDexterAntValue(i);
                    break;
                case SimpleDotsPair::NECK:
                    maxValue = maxSliderValues.getNeckValue(i);
                    break;
                }

                if (maxValue > 170 -  initialPairsValues[i][id]){
                    maxValue = 170 - initialPairsValues[i][id];
                    switch (dot->getType()){
                    case SimpleDotsPair::DEXTER_UP_NORMAL:
                        maxSliderValues.setUpDexterValue(i, maxValue);
                        break;
                    case SimpleDotsPair::SINISTER_DOWN_NORMAL:
                        maxSliderValues.setDownSinisterValue(i, maxValue);
                        break;
                    case SimpleDotsPair::SINISTER_DOWN_ANTAGONIST:
                        maxSliderValues.setDownSinisterAntValue(i, maxValue);
                        break;
                    case SimpleDotsPair::SINISTER_UP_NORMAL:
                        maxSliderValues.setUpSinisterValue(i, maxValue);
                        break;
                    case SimpleDotsPair::SINISTER_UP_ANTAGONIST:
                        maxSliderValues.setUpSinisterAntValue(i, maxValue);
                        break;
                    case SimpleDotsPair::DEXTER_DOWN_NORMAL:
                        maxSliderValues.setDownDexterValue(i, maxValue);
                        break;
                    case SimpleDotsPair::DEXTER_DOWN_ANTAGONIST:
                        maxSliderValues.setDownDexterAntValue(i, maxValue);
                        break;
                    case SimpleDotsPair::DEXTER_UP_ANTAGONIST:
                        maxSliderValues.setUpDexterAntValue(i, maxValue);
                        break;
                    case SimpleDotsPair::NECK:
                        maxSliderValues.setNeckValue(i, maxValue);
                    }
                }
            }
        }
    }

    return maxSliderValues;
}

void MioSuit::readDotsPairsFromCSV()
{
    DotsPair *dotsPair;
    PopDotsPair *popDotsPair;
    QFile file(":/data/ContactsRUENUZ.csv");
    if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
        qDebug() << "File not exists";
    } else {
        QTextStream in(&file);
        int i = 0;
        in.readLine();
        while (!in.atEnd())
        {
            QString line = in.readLine();
            int j = 0;
            int partID = -1, x1 = 0, x2 = 0, y1 = 0, y2 = 0;
            int popX1 = 0, popY1 = 0, popX2 = 0, popY2 = 0;
            int hintX[2], hintY[2];
            bool isFirst = false;
            int sliderX = -1, sliderY = 0;
            QString hint1[3], hint2[3];
            for (const QString &item : line.split(";")) {
                if (item == "") break;
                switch (j) {
                case 1:
                    partID = item.toInt();
                    break;
                case 4:
                    isFirst = item.toInt();
                    break;
                case 5:
                    x1 = item.toInt();
                    break;
                case 6:
                    y1 = item.toInt();
                    break;
                case 7:
                    x2 = item.toInt();
                    break;
                case 8:
                    y2 = item.toInt();
                    break;
                case 9:
                    popX1 = item.toInt();
                    break;
                case 10:
                    popY1 = item.toInt();
                    break;
                case 11:
                    popX2 = item.toInt();
                    break;
                case 12:
                    popY2 = item.toInt();
                    break;
                case 13:
                    hint1[0] = item;
                    break;
                case 14:
                    hint1[1] = item;
                    break;
                case 15:
                    hint1[2] = item;
                    break;
                case 16:
                    hintX[0] = item.toInt();
                    break;
                case 17:
                    hintY[0] = item.toInt();
                    break;
                case 18:
                    hint2[0] = item;
                    break;
                case 19:
                    hint2[1] = item;
                    break;
                case 20:
                    hint2[2] = item;
                    break;
                case 21:
                    hintX[1] = item.toInt();
                    break;
                case 22:
                    hintY[1] = item.toInt();
                    break;
                case 23:
                    pairsNames.append(item);
                    break;
                case 24:
                    sliderX = item.toInt();
                    break;
                case 25:
                    sliderY = item.toInt();
                    break;
                }
                j++;
            }
            pairsValues[0].append(0);
            pairsValues[1].append(0);
            initialPairsValues[0].append(0);
            initialPairsValues[1].append(0);
            dotsPair = new DotsPair(x1, y1, x2, y2, 5, partID, i, isFirst);
            dotsPair->hide();
            dotsPairs.append(dotsPair);
            if (sliderX == -1)
                popDotsPair = new PopDotsPair(popX1, popY1, popX2, popY2, 15, partID, i, isFirst, hint1, hint2, hintX, hintY);
            else
                popDotsPair = new PopDotsPair(popX1, popY1, popX2, popY2, 15, partID, i, isFirst, hint1, hint2, hintX, hintY, sliderX, sliderY);
            popDotsPairs.append(popDotsPair);
            i++;
        }
        file.close();
        QFile file(":/data/ContactsClassification.csv");
        if (!file.open(QFile::ReadOnly | QFile::Text) ) {
            qDebug() << "File not exists";
        } else {
            QTextStream in(&file);
            in.readLine();
            while (!in.atEnd())
            {
                QString line = in.readLine();
                int j = 0;
                int index = 0;
                for (const QString &item : line.split(";")) {
                   switch(j){
                   case 0:
                       index = item.toInt();
                       break;
                   case 3:
                       dotsPairs[index]->setType(static_cast<SimpleDotsPair::PairType>(item.toInt()));
                   }
                   j++;
                }
            }
        }
    }
}

void MioSuit::readHeatersFromCSV()
{
    QFile file(":/data/heaters.csv");
    if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
        qDebug() << "File not exists";
    } else {
        QTextStream in(&file);
        in.readLine();
        while (!in.atEnd())
        {
            QString line = in.readLine();
            int j = 0;
            int id = -1;
            QVector<QString> imgs;
            for (const QString &item : line.split(";")) {
                if (item == "") break;
                switch (j) {
                case 0:
                    id = item.toInt();
                    break;
                default:
                    imgs.append(item);
                }
                j++;
            }
            heaters.append(new Heater(id, imgs));
        }
        file.close();
    }
    QFile file1(":/data/heatPointsRUENUZ.csv");
    if ( !file1.open(QFile::ReadOnly | QFile::Text) ) {
        qDebug() << "File not exists";
    } else {
        QTextStream in(&file1);
        in.readLine();
        int i = 0;
        while (!in.atEnd())
        {
            QString line = in.readLine();
            int j = 0;
            int id = -1;
            QVector<int> x, y, hintsX, hintsY;
            QVector<QString> hintsRU, hintsEN, hintsUZ;
            for (const QString &item : line.split(";")) {
                if (item == "") break;
                switch (j) {
                case 0:
                    id = item.toInt();
                    break;
                default:
                    if (j % 7 == 1){
                       x.append(item.toInt());
                    } else if (j % 7 == 2){
                       y.append(item.toInt());
                    } else if (j % 7 == 3){
                       hintsRU.append(item);
                    } else if (j % 7 == 4){
                       hintsEN.append(item);
                    } else if (j % 7 == 5){
                       hintsUZ.append(item);
                    } else if (j % 7 == 6){
                       hintsX.append(item.toInt());
                    } else{
                       hintsY.append(item.toInt());
                    }
                }
                j++;
            }
            for(qsizetype i = 0; i < y.size(); i++){
                heaters[id]->addDot(x[i], y[i], 11);
                heaters[id]->addHint(hintsX[i], hintsY[i], hintsRU[i], hintsEN[i], hintsUZ[i]);
            }
            i++;
        }
        file1.close();
    }
}
