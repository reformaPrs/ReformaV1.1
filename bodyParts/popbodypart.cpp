#include "popbodypart.h"

PopBodyPart::PopBodyPart(int id, QString img1[5], QString img2[5]):
    AbstractBodyPart(id)
{
    const qreal backgroundOpacity = 0.9;
    const QPoint offset(IMPULSE_ADDER_OFFSET);
    firstBackground = new HoverPixmapItem(QPixmap(img1[Background]).scaledToWidth(SCALED_WIDTH));
    secondBackground = new HoverPixmapItem(QPixmap(img2[Background]).scaledToWidth(SCALED_WIDTH));
    firstBackground->setOpacity(backgroundOpacity);
    secondBackground->setOpacity(backgroundOpacity);
    connect(firstBackground, SIGNAL(mouseClicked()), this, SLOT(onBackgroundClicked()));
    connect(secondBackground, SIGNAL(mouseClicked()), this, SLOT(onBackgroundClicked()));
    firstBackground->setOffset(offset);
    secondBackground->setOffset(firstBackground->boundingRect().width()+offset.x(), offset.y());

    for(int i = 0;  i < Images-1; i++){
        firstMuscle.append(new HoverPixmapItem(QPixmap(img1[i]).scaledToWidth(SCALED_WIDTH)));
        firstMuscle.last()->setOffset(offset);
        secondMuscle.append(new HoverPixmapItem(QPixmap(img2[i]).scaledToWidth(SCALED_WIDTH)));
        secondMuscle.last()->setOffset(firstBackground->boundingRect().width()+offset.x(), offset.y());
        firstMuscle.last()->hide();
        secondMuscle.last()->hide();
    }
    initMuscles();
    status = BothDisabled;
}

PopBodyPart::~PopBodyPart()
{
    delete firstBackground;
    delete secondBackground;
    for (auto muscle : firstMuscle){
        delete muscle;
    }
    for (auto muscle : secondMuscle){
        delete muscle;
    }
}

void PopBodyPart::addToScene(QGraphicsScene *scene)
{
    scene->addItem(firstBackground);
    scene->addItem(secondBackground);
    for(int i = 0;  i < Images-1; i++){
        scene->addItem(firstMuscle[i]);
        scene->addItem(secondMuscle[i]);
    }
}

void PopBodyPart::removeFromScene(QGraphicsScene *scene)
{
    scene->removeItem(firstBackground);
    scene->removeItem(secondBackground);
    for(int i = 0;  i < Images-1; i++){
        scene->removeItem(firstMuscle[i]);
        scene->removeItem(secondMuscle[i]);
    }
}

void PopBodyPart::initMuscles()
{

    firstMuscle[Highlight]->setAcceptHoverEvents(true);
    secondMuscle[Highlight]->setAcceptHoverEvents(true);
    firstMuscle[Highlight]->setOpacity(0.001);
    secondMuscle[Highlight]->setOpacity(0.001);
    connect(firstMuscle[Highlight], SIGNAL(mouseClicked()), this, SLOT(onMuscleClicked()));
    connect(firstMuscle[Highlight], SIGNAL(hoverEntered()), this, SLOT(onHoverEntered()));
    connect(firstMuscle[Highlight], SIGNAL(hoverLeft()), this, SLOT(onHoverLeft()));
    connect(secondMuscle[Highlight], SIGNAL(mouseClicked()), this, SLOT(onMuscleClicked()));
    connect(secondMuscle[Highlight], SIGNAL(hoverEntered()), this, SLOT(onHoverEntered()));
    connect(secondMuscle[Highlight], SIGNAL(hoverLeft()), this, SLOT(onHoverLeft()));
    firstMuscle[Highlight]->show();
    secondMuscle[Highlight]->show();
}

void PopBodyPart::setStatus(enum Status newStatus)
{
    status = newStatus;
    switch (status) {
    case Intervals:
        break;
    case SecondEnabled:
        firstMuscle[Green]->hide();
        secondMuscle[Green]->show();
        firstMuscle[Red]->show();
        secondMuscle[Red]->hide();
        firstMuscle[Yellow]->hide();
        secondMuscle[Yellow]->hide();
        break;
    case BothEnabled:
        firstMuscle[Green]->hide();
        secondMuscle[Green]->hide();
        firstMuscle[Red]->hide();
        secondMuscle[Red]->hide();
        firstMuscle[Yellow]->show();
        secondMuscle[Yellow]->show();
        break;
    case BothDisabled:
        firstMuscle[Green]->hide();
        secondMuscle[Green]->hide();
        firstMuscle[Red]->hide();
        secondMuscle[Red]->hide();
        firstMuscle[Yellow]->hide();
        secondMuscle[Yellow]->hide();
        break;
    case FirstEnabled:
        firstMuscle[Green]->show();
        secondMuscle[Green]->hide();
        secondMuscle[Red]->show();
        firstMuscle[Red]->hide();
        firstMuscle[Yellow]->hide();
        secondMuscle[Yellow]->hide();
    }
}

void PopBodyPart::onMuscleClicked()
{
    wasMuscleClicked = true;
    if (sender() == firstMuscle[Highlight]) {
        if(firstMuscle[Green]->isVisible()){
            setStatus(BothEnabled);
        } else if (firstMuscle[Red]->isVisible()){
            setStatus(BothDisabled);
        } else if (firstMuscle[Yellow]->isVisible()){
            setStatus(FirstEnabled);
        } else {
            setStatus(SecondEnabled);
        }
        emit clicked(true);
    } else {
        if(secondMuscle[Green]->isVisible()){
            setStatus(BothEnabled);
        } else if (secondMuscle[Red]->isVisible()){
            setStatus(BothDisabled);
        } else if (secondMuscle[Yellow]->isVisible()){
            setStatus(SecondEnabled);
        } else {
            setStatus(FirstEnabled);
        }
        emit clicked(false);
    }
}

void PopBodyPart::onHoverEntered()
{
    firstMuscle[Highlight]->setOpacity(1);
    secondMuscle[Highlight]->setOpacity(1);
}

void PopBodyPart::onHoverLeft()
{
    firstMuscle[Highlight]->setOpacity(0.001);
    secondMuscle[Highlight]->setOpacity(0.001);
}

void PopBodyPart::onBackgroundClicked()
{
    if (!wasMuscleClicked) {
        emit backgroundClicked();
    } else {
        wasMuscleClicked = false;
    }
}
