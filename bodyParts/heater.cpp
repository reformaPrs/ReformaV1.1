#include "heater.h"

Heater::Heater(int id, QVector<QString> imgs, QObject *parent):
    QObject(parent),
    id(id)
{
    for(const auto &img : imgs){
        HoverPixmapItem *tmp = new HoverPixmapItem(QPixmap(img));
        tmp->setOpacity(0.5);
        tmp->setAcceptHoverEvents(true);
        tmp->hide();
        connect(tmp, SIGNAL(mouseClicked()), this, SLOT(onHeaterClick()));
        heatPoints.append(tmp);
    }
    isActive = false;
}

Heater::~Heater()
{
    for(auto point : heatPoints){
        delete point;
    }
    for(auto dot : dots){
        delete dot;
    }
    for(auto hint : hints){
        delete hint;
    }
}

void Heater::hide()
{
    for(auto point : heatPoints){
        point->hide();
    }
    for(auto dot : dots){
        dot->hide();
    }
}

void Heater::show()
{
    if (isActive) {
        for(auto point : heatPoints){
            point->show();
        }
    } else {
        for(auto dot : dots){
            dot->show();
        }
    }
}

bool Heater::getIsActive() const
{
    return isActive;
}

void Heater::setIsActive(bool newIsActive)
{
    isActive = newIsActive;
    if(isActive){
        for(auto point : heatPoints) {
            point->setOpacity(1);
        }
    } else {
        for(auto point : heatPoints) {
            point->setOpacity(0.5);
        }
    }
    if(dots[0]->isVisible() || heatPoints[0]->isVisible()){
        if(isActive) {
            for(auto point : heatPoints) {
                point->show();
            }
            for(auto dot : dots){
                dot->hide();
            }
        } else {
            for(auto point : heatPoints){

                point->hide();
            }
            for(auto dot : dots){
                dot->show();
            }
        }
    }
}

void Heater::addDot(int x, int y, int r)
{
    Dot *tmp = new Dot(x, y, r);
    tmp->hide();
    tmp->setAcceptHoverEvents(true);

    connect(tmp, SIGNAL(clicked()), this, SLOT(onHeaterClick()));

    connect(tmp, SIGNAL(hoverEntered()), this, SLOT(onDotEntered()));
    connect(tmp, SIGNAL(hoverLeft()), this, SLOT(onDotLeft()));
    tmp->setBrush(QColor(200, 200, 200));
    tmp->setPen(QPen(QColor(70, 108, 128), 2));
    tmp->setOpacity(0.8);
    dots.append(tmp);
}

void Heater::addHint(int x, int y, QString hintRU, QString hintEN)
{
    hintsRU.append(hintRU);
    hintsEN.append(hintEN);
    TextRect *textRect = new TextRect(hintRU, x, y, TextRect::Center);
    textRect->setFont(QFont("Arial Rounded MT Bold", 11, QFont::Medium));
    textRect->hide();
    textRect->setWidth(600);
    hints.append(textRect);
}

void Heater::addToScene(QGraphicsScene *scene)
{
    for(auto point : heatPoints){
        scene->addItem(point);
    }
    for(auto dot : dots){
        scene->addItem(dot);
    }
    for(auto hint : hints){
        hint->addToScene(scene);
    }
}

void Heater::translate(Language language)
{
    if (language == RU)
        for(qsizetype i = 0; i < hints.size(); i++){
            hints[i]->setText(hintsRU[i]);
        }
    else
        for(qsizetype i = 0; i < hints.size(); i++){
            hints[i]->setText(hintsEN[i]);
        }
}

void Heater::setIsAdminModeActive(bool newIsAdminModeActive)
{
    isAdminModeActive = newIsAdminModeActive;
}

void Heater::onHeaterClick()
{
    if (!isAdminModeActive) return;
    emit clicked(id);
    isActive = !isActive;
    if(isActive) {
        for(auto point : heatPoints) {
            point->setOpacity(1);
            point->show();
        }
        for (auto dot : dots){
            dot->hide();
        }
        for (auto hint : hints){
            hint->hide();
        }
    } else {
        for(auto point : heatPoints){
            point->setOpacity(0.5);
            point->hide();
        }
        for(auto dot : dots){
            dot->show();
        }
    }
}

void Heater::onDotEntered()
{
    for (qsizetype i = 0; i < dots.size(); i++){
        if (dots[i] == sender()){
            hints[i]->show();
        }
    }
    if(!isActive)
        for(auto point : heatPoints) {
            point->setOpacity(0.5);
            point->show();
        }
}

void Heater::onDotLeft()
{
    for (qsizetype i = 0; i < dots.size(); i++){
        if (dots[i] == sender()){
            hints[i]->hide();
        }
    }
    if(!isActive)
        for(auto point : heatPoints)
            point->hide();
}

