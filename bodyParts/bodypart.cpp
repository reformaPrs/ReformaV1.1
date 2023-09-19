#include "bodypart.h"

BodyPart::BodyPart(int id, QString img1[5], QString img2[5]):
    AbstractBodyPart(id)
{
    for (int i = 0; i < Images; i++) {
        firstMuscle.append(new HoverPixmapItem(QPixmap(img1[i])));
        secondMuscle.append(new HoverPixmapItem(QPixmap(img2[i])));
        firstMuscle.last()->hide();
        secondMuscle.last()->hide();
    }

    initMuscles();
    status = BothDisabled;
}

BodyPart::~BodyPart()
{
    for (auto muscle : firstMuscle){
        delete muscle;
    }
    for (auto muscle : secondMuscle){
        delete muscle;
    }
    delete hintTextRect1;
    delete hintTextRect2;
}


void BodyPart::addHints(QString *hint1, QString *hint2, int hintX1, int hintY1, int hintX2, int hintY2)
{
    for (int i = 0; i < 3; i++) {
        hintStr1[i] = hint1[i];
        hintStr2[i] = hint2[i];
    }

    hintTextRect1 = new TextRect(hint1[RU], hintX1, hintY1, TextRect::Center);
    hintTextRect2 = new TextRect(hint2[RU], hintX2, hintY2, TextRect::Center);
    hintTextRect1->setFont(QFont("Arial Rounded MT Bold", 12, QFont::Medium));
    hintTextRect2->setFont(QFont("Arial Rounded MT Bold", 12, QFont::Medium));
    hintTextRect1->hide();
    hintTextRect2->hide();
}

void BodyPart::addToScene(QGraphicsScene *scene){
    for (int i = 0; i < Images; i++) {
        scene->addItem(firstMuscle[i]);
        scene->addItem(secondMuscle[i]);
    }


    hintTextRect1->addToScene(scene);
    hintTextRect2->addToScene(scene);
}

void BodyPart::initMuscles()
{
    firstMuscle[Yellow]->setAcceptHoverEvents(true);
    secondMuscle[Yellow]->setAcceptHoverEvents(true);
    firstMuscle[Yellow]->setOpacity(0.001);
    secondMuscle[Yellow]->setOpacity(0.001);
    connect(firstMuscle[Highlight], SIGNAL(mouseClicked()), this, SLOT(onMuscleClick()));
    connect(firstMuscle[Yellow], SIGNAL(hoverEntered()), this, SLOT(onHoverEntered()));
    connect(firstMuscle[Yellow], SIGNAL(hoverLeft()), this, SLOT(onHoverLeft()));
    connect(secondMuscle[Highlight], SIGNAL(mouseClicked()), this, SLOT(onMuscleClick()));
    connect(secondMuscle[Yellow], SIGNAL(hoverEntered()), this, SLOT(onHoverEntered()));
    connect(secondMuscle[Yellow], SIGNAL(hoverLeft()), this, SLOT(onHoverLeft()));
    firstMuscle[Yellow]->show();
    secondMuscle[Yellow]->show();
}

bool BodyPart::getIntaractive() const
{
    return intaractive;
}

void BodyPart::setIntaractive(bool newIntaractive)
{
    intaractive = newIntaractive;
}

void BodyPart::translate(Language language)
{
    hintTextRect1->setText(hintStr1[language]);
    hintTextRect2->setText(hintStr2[language]);
}



void BodyPart::hide()
{
    for(int i = 0; i < Images; i++){
        firstMuscle[i]->hide();
        secondMuscle[i]->hide();
    }
    hintTextRect1->hide();
    hintTextRect2->hide();
}

void BodyPart::show()
{
    firstMuscle[Yellow]->show();
    secondMuscle[Yellow]->show();
    setStatus(status);
}

void BodyPart::setStatus(enum Status newStatus)
{
    status = newStatus;
    switch (status) {
    case SecondEnabled:
        firstMuscle[Green]->hide();
        secondMuscle[Green]->show();
        firstMuscle[Red]->show();
        secondMuscle[Red]->hide();
        firstMuscle[Interval]->hide();
        secondMuscle[Interval]->hide();
        firstMuscle[Yellow]->setOpacity(0.001);
        secondMuscle[Yellow]->setOpacity(0.001);
        break;
    case BothEnabled:
        firstMuscle[Green]->hide();
        secondMuscle[Green]->hide();
        firstMuscle[Red]->hide();
        secondMuscle[Red]->hide();
        firstMuscle[Interval]->hide();
        secondMuscle[Interval]->hide();
        firstMuscle[Yellow]->setOpacity(1);
        secondMuscle[Yellow]->setOpacity(1);
        break;
    case BothDisabled:
        firstMuscle[Green]->hide();
        secondMuscle[Green]->hide();
        firstMuscle[Red]->hide();
        secondMuscle[Red]->hide();
        firstMuscle[Interval]->hide();
        secondMuscle[Interval]->hide();
        firstMuscle[Yellow]->setOpacity(0.001);
        secondMuscle[Yellow]->setOpacity(0.001);
        break;
    case FirstEnabled:
        firstMuscle[Green]->show();
        secondMuscle[Green]->hide();
        firstMuscle[Red]->hide();
        secondMuscle[Red]->show();
        firstMuscle[Interval]->hide();
        secondMuscle[Interval]->hide();
        firstMuscle[Yellow]->setOpacity(0.001);
        secondMuscle[Yellow]->setOpacity(0.001);
        break;
    case Intervals:
        firstMuscle[Green]->hide();
        secondMuscle[Green]->hide();
        firstMuscle[Red]->hide();
        secondMuscle[Red]->hide();
        firstMuscle[Interval]->show();
        secondMuscle[Interval]->show();
        firstMuscle[Yellow]->setOpacity(0.001);
        secondMuscle[Yellow]->setOpacity(0.001);
        break;
    }
}

void BodyPart::onMuscleClick()
{
    if (intaractive)
        emit clicked(getId());
}

void BodyPart::onHoverEntered()
{
    if (intaractive) {
        firstMuscle[Highlight]->show();
        secondMuscle[Highlight]->show();
        if (sender() == firstMuscle[Yellow]) {
            hintTextRect1->show();
        } else {
            hintTextRect2->show();
        }
    }
}

void BodyPart::onHoverLeft()
{
    if (intaractive) {
        firstMuscle[Highlight]->hide();
        secondMuscle[Highlight]->hide();
        hintTextRect1->hide();
        hintTextRect2->hide();
    }
}

