#include "dotspair.h"


DotsPair::DotsPair(int x1, int y1, int x2, int y2, int radius, int partID, int id, bool firstMuscle):
    SimpleDotsPair(partID, id),
    x1(x1),
    x2(x2),
    y1(y1),
    y2(y2),
    radius(radius),
    isFirstMuscle(firstMuscle)
{
    textOffset = QPoint(0, 0);
    dot1 = new Dot(x1, y1, radius);
    dot2 = new Dot(x2, y2, radius);
    rect = new QGraphicsRectItem((x1 + x2) / 2 - 2 * radius,
                                 (y1 + y2) / 2 - radius,
                                 4 * radius, 2 * radius);
    line = new QGraphicsLineItem(x1, y1, x2, y2);
    text = new QGraphicsTextItem();
    text->setFont(QFont("Arial", 10, QFont::Medium));

    dot1->setBrush(QColor(DOT_COLOR));
    dot1->setPen(QPen(QColor(70, 108, 128), 2));
    dot2->setBrush(dot1->brush());
    dot2->setPen(dot1->pen());
    line->setPen(QPen(QColor(108, 121, 128, 200), 2));
    rect->setBrush(QBrush(QColor(217, 242, 255)));
    rect->setPen(QPen(QColor(70, 108, 128), 1));

    rect->hide();
}

DotsPair::~DotsPair()
{
    delete dot1;
    delete dot2;
    delete rect;
    delete line;
    delete text;
}


void DotsPair::addToScene(QGraphicsScene *scene)
{
    scene->addItem(line);
    scene->addItem(dot1);
    scene->addItem(dot2);
    scene->addItem(rect);
    scene->addItem(text);
    if (getValue() != 0) {
        dot1->setBrush(QColor(ACTIVE_DOT_COLOR));
        dot2->setBrush(dot1->brush());
        text->setPlainText(QString::number(getValue()));
        int h = text->boundingRect().height();
        int w = text->boundingRect().width();
        text->setPos((x1 + x2) / 2 + textOffset.x() - w / 2,
                     (y1 + y2) / 2 + textOffset.y() - h / 2);
        h+=0;
        w+=0;
        rect->setRect((x1 + x2) / 2 + textOffset.x() - w / 2,
                      (y1 + y2) / 2 + textOffset.y() - h / 2,
                      w, h);
        text->show();
        rect->show();
    }
}

void DotsPair::removeFromScene(QGraphicsScene *scene)
{
    scene->removeItem(line);
    scene->removeItem(dot1);
    scene->removeItem(dot2);
    scene->removeItem(rect);
    scene->removeItem(text);
}

void DotsPair::setAcceptHoverEvents(bool hover)
{
    dot1->setAcceptHoverEvents(hover);
    dot2->setAcceptHoverEvents(hover);
}

int DotsPair::getRadius() const
{
    return radius;
}

Dot *DotsPair::getDot1() const
{
    return dot1;
}

Dot *DotsPair::getDot2() const
{
    return dot2;
}

void DotsPair::setOffset(QPoint offset)
{
    dot1->setOffset(offset);
    dot2->setOffset(offset);
    line->setPos(offset + line->pos());
    textOffset = offset;
}

void DotsPair::hide()
{
    rect->hide();
    line->hide();
    dot1->hide();
    dot2->hide();
    text->hide();
}

void DotsPair::show()
{
    if (getValue() != 0) {
        dot1->setBrush(QColor(ACTIVE_DOT_COLOR));
        dot2->setBrush(dot1->brush());
        text->setPlainText(QString::number(getValue()));
        int h = text->boundingRect().height();
        int w = text->boundingRect().width();
        text->setPos((x1 + x2) / 2 + textOffset.x() - w / 2,
                     (y1 + y2) / 2 + textOffset.y() - h / 2);
        h+=0;
        w+=0;
        rect->setRect((x1 + x2) / 2 + textOffset.x() - w / 2,
                      (y1 + y2) / 2 + textOffset.y() - h / 2,
                      w, h);
        text->show();
        rect->show();
    } else {
        dot1->setBrush(QColor(DOT_COLOR));
        dot2->setBrush(dot1->brush());
        text->hide();
        rect->hide();
    }
    line->show();
    dot1->show();
    dot2->show();
}



void DotsPair::setValue(int newValue)
{
    if (newValue >= 25 || newValue == 0) {
        SimpleDotsPair::setValue(newValue);
        text->setPlainText(QString::number(newValue));
        int h = text->boundingRect().height();
        int w = text->boundingRect().width();
        text->setPos((x1 + x2) / 2 + textOffset.x() - w / 2,
                     (y1 + y2) / 2 + textOffset.y() - h / 2);
        h+=2;
        w+=2;
        rect->setRect((x1 + x2) / 2 + textOffset.x() - w / 2,
                      (y1 + y2) / 2 + textOffset.y() - h / 2,
                      w, h);
    }
}

bool DotsPair::getIsFirstMuscle() const
{
    return isFirstMuscle;
}



