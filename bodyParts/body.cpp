#include "body.h"


Body *Body::getBodyInstance()
{
    static Body *instance;
    if (instance != nullptr){
        return instance;
    }
    instance = new Body();

    return instance;
}

Body::~Body()
{
    for (auto part : bodyParts){
        delete part;
    }
    for (auto part : popBodyParts){
        delete part;
    }
    delete body;
}

void Body::addToScene(QGraphicsScene *scene)
{
    scene->addItem(body);
    //scene->setSceneRect(body->boundingRect());
    for (auto part : bodyParts){
        part->addToScene(scene);
    }
}

void Body::addPopToScene(QGraphicsScene *scene, int id, const QObject *dialog)
{
    if (bodyParts[id]->getStatus() == AbstractBodyPart::Intervals){
        popBodyParts[id]->setStatus(AbstractBodyPart::BothDisabled);
    } else {
        popBodyParts[id]->setStatus(bodyParts[id]->getStatus());
    }
    popBodyParts[id]->addToScene(scene);
    connect(popBodyParts[id], SIGNAL(clicked(bool)), dialog, SLOT(onBodyPartClicked(bool)));
    connect(popBodyParts[id], SIGNAL(backgroundClicked()), dialog, SLOT(onBackgroundClicked()));
}

void Body::removePopFromScene(QGraphicsScene *scene, int id)
{
    popBodyParts[id]->removeFromScene(scene);
}

void Body::mergeChanges(int id){
    bodyParts[id]->setStatus(popBodyParts[id]->getStatus());
}

void Body::translate(Language language)
{
    if (language == RU) {
        for (auto part : bodyParts) {
            part->translate(RU);
        }
    } else {
        for (auto part : bodyParts){
            part->translate(EN);
        }
    }
}

AbstractBodyPart::Status Body::getPopStatus(int id)const
{
    return popBodyParts[id]->getStatus();
}

void Body::setPopStatus(int id, AbstractBodyPart::Status status)
{
    popBodyParts[id]->setStatus(status);
}

void Body::setPartStatus(int id, AbstractBodyPart::Status status)
{
    bodyParts[id]->setStatus(status);
}


void Body::hide()
{
    for (auto part : bodyParts){
        part->hide();
    }
}

void Body::show()
{
    for (auto part : bodyParts){
        part->show();
    }
}

void Body::setIntaractive(ActiveBodyPart activePart)
{
    switch (activePart) {
    case WHOLE_BODY:
        for (auto part : bodyParts) {
            if (part->getStatus() != AbstractBodyPart::Intervals)
                part->setIntaractive(true);
            else
                part->setIntaractive(false);
        }
        break;
    case UP_BODY:
        for (int i = 0; i < 18; i++) {
            if (i < 6 || i > 15) {
                bodyParts[i]->setIntaractive(false);
            } else {
                bodyParts[i]->setIntaractive(true);
            }
        }
        break;
    case DOWN_BODY:
        for (int i = 0; i < 18; i++) {
            if (i < 6 || i > 15) {
                bodyParts[i]->setIntaractive(true);
            } else {
                bodyParts[i]->setIntaractive(false);
            }
        }
        break;
    case NONE:
        for (auto part : bodyParts) {
            part->setIntaractive(false);
        }
        break;
    }
}

Body::Body(QObject *parent)
    : QObject{parent}
{
    body = new HoverPixmapItem(QPixmap(":/body/bodyBlue.jpg"));
    connect(body, SIGNAL(mouseClicked()), this, SLOT(onBackgroundClicked()));
    //Добавление частей тела с информацией из CSV
    readBodyPartsFromCSV();
    //Чтение частей тела для всплывающих окон из CSV
    readPopBodyPartsFromCSV();
}

void Body::setIsAdminModeActive(bool newIsAdminModeActive)
{
    isAdminModeActive = newIsAdminModeActive;
}

void Body::setHalfhourIndex(int newHalfhourIndex)
{
    if (newHalfhourIndex != 0 &&
            newHalfhourIndex != 1){
        throw HalfHourException();
    }
    halfhourIndex = newHalfhourIndex;
}

void Body::saveHalfhour()
{
    if (isHalfhourActive){
        for (qsizetype i = 0; i < 18; i++) {
            bodyPartsStatuses[halfhourIndex][i] = bodyParts[i]->getStatus();
        }
        return;
    }
    for (qsizetype i = 0; i < 18; i++) {
        bodyPartsStatuses[0][i] = bodyParts[i]->getStatus();
        bodyPartsStatuses[1][i] = bodyParts[i]->getStatus();
    }
}

void Body::resetIntervalParts()
{
    for (qsizetype i = 0; i < 18; i++) {
        bodyParts[i]->setStatus(bodyPartsStatuses[halfhourIndex][i]);
        if (bodyParts[i]->getStatus() == AbstractBodyPart::Intervals) {
            bodyParts[i]->setStatus(AbstractBodyPart::BothDisabled);
        }
    }
}

void Body::setCalculatePartStatus(int partID, bool isFirst)
{
    AbstractBodyPart::Status status = bodyParts[partID]->getStatus();
    if (isFirst) {
        if (status == AbstractBodyPart::BothDisabled){
            bodyParts[partID]->setStatus(AbstractBodyPart::FirstEnabled);
        } else if (status == AbstractBodyPart::SecondEnabled){
            bodyParts[partID]->setStatus(AbstractBodyPart::BothEnabled);
        }
    } else {
        if (status == AbstractBodyPart::BothDisabled){
            bodyParts[partID]->setStatus(AbstractBodyPart::SecondEnabled);
        } else if (status == AbstractBodyPart::FirstEnabled){
            bodyParts[partID]->setStatus(AbstractBodyPart::BothEnabled);
        }
    }
}

void Body::copyHalfhour()
{
    saveHalfhour();
    int otherIndex = 0;
    if (otherIndex == halfhourIndex){
        otherIndex = 1;
    }
    qDebug() << otherIndex;
    for (int i = 0; i < 18; i++){
        bodyPartsStatuses[otherIndex][i] = bodyPartsStatuses[halfhourIndex][i];
    }
}

void Body::clearHalfhour()
{
    if (isHalfhourActive){
        for (int i = 0; i < 18; i++){
            bodyPartsStatuses[halfhourIndex][i] = AbstractBodyPart::BothDisabled;
        }
    } else {
        for (int i = 0; i < 18; i++){
            bodyPartsStatuses[0][i] = AbstractBodyPart::BothDisabled;
            bodyPartsStatuses[1][i] = AbstractBodyPart::BothDisabled;
        }
    }
    update();
}

void Body::clear()
{
    for (int i = 0; i < 18; i++){
        bodyPartsStatuses[0][i] = AbstractBodyPart::BothDisabled;
        bodyPartsStatuses[1][i] = AbstractBodyPart::BothDisabled;
    }
}

void Body::update()
{
    for (qsizetype i = 0; i < 18; i++) {
        bodyParts[i]->setStatus(bodyPartsStatuses[halfhourIndex][i]);
    }
}

void Body::update(const QVector<DotsPair *> dotsPairs)
{
    for (auto pair : dotsPairs) {
        if (pair->getValue() == 0) continue;
        int partID = pair->getPartID();
        AbstractBodyPart::Status status = bodyParts[partID]->getStatus();
        if (pair->getIsFirstMuscle()) {
            if (status == AbstractBodyPart::BothDisabled){
                bodyParts[partID]->setStatus(AbstractBodyPart::FirstEnabled);
            } else if (status == AbstractBodyPart::SecondEnabled){
                bodyParts[partID]->setStatus(AbstractBodyPart::BothEnabled);
            }
        } else {
            if (status == AbstractBodyPart::BothDisabled){
                bodyParts[partID]->setStatus(AbstractBodyPart::SecondEnabled);
            } else if (status == AbstractBodyPart::FirstEnabled){
                bodyParts[partID]->setStatus(AbstractBodyPart::BothEnabled);
            }
        }
    }
}

void Body::update(const QVector<IntervalDotsPair *> IntervalDotsPairs)
{
    for (auto pair : IntervalDotsPairs){
        bodyParts[pair->getPartID()]->setStatus(AbstractBodyPart::Intervals);
    }
}

void Body::saveToFile(QDataStream *out)
{
    for (auto status : bodyPartsStatuses[0]){
        *out << status;
    }
    if (isHalfhourActive) {
        for (auto status : bodyPartsStatuses[1]){
            *out << status;
        }
    } else {
        for (auto status : bodyPartsStatuses[0]){
            *out << status;
        }
    }
}

void Body::readFromFile(QDataStream *in)
{
    for (int i = 0; i < 18; i++){
        *in >> bodyPartsStatuses[0][i];
    }
    for (int i = 0; i < 18; i++){
        *in >> bodyPartsStatuses[1][i];
    }
    bool isEquals = true;
    for (int i = 0; i < 18; i++){
        if (bodyPartsStatuses[1][i] != bodyPartsStatuses[0][i]) {
            isEquals = false;
            break;
        }
    }
    isHalfhourActive = !isEquals;
}

void Body::readBodyPartsFromCSV()
{
    QFile file(":/data/parts.csv");
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "File not exists";
        return;
    }

    QString content = file.readAll();
    file.close();
    QStringList lines = content.split("\n");

    // Skip header
    lines.removeFirst();

    int i = 0;
    for (const QString &line : lines) {
        if (line.trimmed().isEmpty()) continue;  // Skip empty lines

        int j = 0;
        QString img1[5], img2[5], hint1[2], hint2[2];
        int x1 = -1, y1 = -1, x2 = -1, y2 = -1;

        for (const QString &item : line.split(";")) {
            if (item == "") break;
            if (j > 0 && j < 11) {
                if (j > 0 && j%2 == 0) {
                    img2[j/2 - 1] = item;
                } else if (j > 0) {
                    img1[j/2] = item;
                }
            } else {
                switch (j){
                case 11:
                    hint1[0] = item;
                    break;
                case 12:
                    hint1[1] = item;
                    break;
                case 13:
                    x1 = item.toInt();
                    break;
                case 14:
                    y1 = item.toInt();
                    break;
                case 15:
                    hint2[0] = item;
                    break;
                case 16:
                    hint2[1] = item;
                    break;
                case 17:
                    x2 = item.toInt();
                    break;
                case 18:
                    y2 = item.toInt();
                    break;
                }
            }
            j++;
        }

        bodyPartsStatuses[0][i] = AbstractBodyPart::BothDisabled;
        bodyPartsStatuses[1][i] = AbstractBodyPart::BothDisabled;
        bodyParts[i] = new BodyPart(i, img1, img2);
        bodyParts[i]->addHints(hint1, hint2, x1, y1, x2, y2);

        connect(bodyParts[i], SIGNAL(clicked(int)), this, SLOT(onBodyPartClicked(int)));

        i++;
    }
}

void Body::readPopBodyPartsFromCSV()
{
    QFile file(":/data/pops.csv");
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "File not exists";
        return;
    }

    QString content = file.readAll();
    file.close();
    QStringList lines = content.split("\n");

    // Skip header
    lines.removeFirst();

    int i = 0;
    for (const QString &line : lines) {
        if (line.trimmed().isEmpty()) continue;  // Skip empty lines

        int j = 0;
        QString img1[5], img2[5];

        for (const QString &item : line.split(";")) {
            if (item == "") break;
            if (j > 0 && j%2 == 0) {
                img2[j/2 - 1] = item;
            } else if (j > 0) {
                img1[j/2] = item;
            }
            j++;
        }

        popBodyParts[i] = new PopBodyPart(i, img1, img2);
        i++;
    }
}



//void Body::readBodyPartsFromCSV()
//{
//    QFile file(":/data/parts.csv");
//    if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
//        qDebug() << "File not exists";
//    } else {
//        QTextStream in(&file);
//        int i = 0;
//        in.readLine();
//        while (!in.atEnd())
//        {
//            QString line = in.readLine();
//            int j = 0;
//            QString img1[5], img2[5], hint1[2], hint2[2];
//            int x1 = -1, y1 = -1, x2 = -1, y2 = -1;
//            for (const QString &item : line.split(";")) {
//                if (item == "") break;
//                if (j > 0 && j < 11) {
//                    if (j > 0 && j%2 == 0) {
//                        img2[j/2 - 1] = item;
//                    } else if (j > 0) {
//                        img1[j/2] = item;
//                    }
//                } else {
//                    switch (j){
//                    case 11:
//                        hint1[0] = item;
//                        break;
//                    case 12:
//                        hint1[1] = item;
//                        break;
//                    case 13:
//                        x1 = item.toInt();
//                        break;
//                    case 14:
//                        y1 = item.toInt();
//                        break;
//                    case 15:
//                        hint2[0] = item;
//                        break;
//                    case 16:
//                        hint2[1] = item;
//                        break;
//                    case 17:
//                        x2 = item.toInt();
//                        break;
//                    case 18:
//                        y2 = item.toInt();
//                        break;
//                    }
//                }
//                j++;
//            }
//            bodyPartsStatuses[0][i] = AbstractBodyPart::BothDisabled;
//            bodyPartsStatuses[1][i] = AbstractBodyPart::BothDisabled;
//            bodyParts[i] = new BodyPart(i, img1, img2);
//            bodyParts[i]->addHints(hint1, hint2, x1, y1, x2, y2);

//            connect(bodyParts[i], SIGNAL(clicked(int)), this, SLOT(onBodyPartClicked(int)));

//            i++;
//        }
//        file.close();
//    }
//}

//void Body::readPopBodyPartsFromCSV()
//{
//    QFile file(":/data/pops.csv");
//    if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
//        qDebug() << "File not exists";
//    } else {
//        QTextStream in(&file);
//        int i = 0;
//        in.readLine();
//        while (!in.atEnd())
//        {
//            QString line = in.readLine();
//            int j = 0;
//            QString img1[5], img2[5];
//            for (const QString &item : line.split(";")) {
//                if (item == "") break;
//                if (j > 0 && j%2 == 0) {
//                    img2[j/2 - 1] = item;
//                } else if (j > 0) {
//                    img1[j/2] = item;
//                }
//                j++;
//            }
//            popBodyParts[i] = new PopBodyPart(i, img1, img2);
//            i++;
//        }
//        file.close();
//    }
//}

void Body::onBodyPartClicked(int id) {
    qDebug() << "Body::bodyPartClicked";
    qDebug() << isAdminModeActive;
    if (!isAdminModeActive) return;
    bodyParts[id]->onHoverLeft();
    if(bodyParts[id]->getStatus() != AbstractBodyPart::Intervals){
        popBodyParts[id]->setStatus(bodyParts[id]->getStatus());
    } else {
        popBodyParts[id]->setStatus(AbstractBodyPart::BothDisabled);
    }
    oldPopStatus = popBodyParts[id]->getStatus();//Копируем статус всплывающей части тела
    emit clickedBodyPart(id);
}

void Body::onBackgroundClicked()
{
    emit clickedBackground();
}

void Body::activateHalfhour()
{
    isHalfhourActive = true;
}

void Body::deactivateHalfhour()
{
    isHalfhourActive = false;
}
