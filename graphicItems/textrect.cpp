#include "textrect.h"

TextRect::TextRect(QString str, int x, int y, Alignment alignment):
    alignment(alignment), x(x), y(y)
{
    text = new QGraphicsTextItem();
    rect = new QGraphicsRectItem();

    text->setFont(QFont("Segoe UI Bold", 13));
    text->setPlainText(str);
    width = 600;
    text->setTextWidth(width);
    text->setZValue(1000);
    rect->setZValue(1000);
    int h = text->boundingRect().height();
    int w = text->boundingRect().width();
    if (alignment == Center){
        rect->setRect(x - w / 2, y - h / 2, w, h);
        text->setPos(x - w / 2, y - h / 2);
    } else {
        rect->setRect(x, y, w, h);
        text->setPos(x + 2, y);
    }

    rect->setBrush(QColor(255, 255, 255, 200));
    rect->setPen(QPen(QColor(0, 0, 0, 200), 2));
}

TextRect::~TextRect()
{
    delete text;
    delete rect;
}

void TextRect::addToScene(QGraphicsScene *scene)
{
    scene->addItem(rect);
    scene->addItem(text);
}

void TextRect::removeFromScene(QGraphicsScene *scene)
{
    scene->removeItem(rect);
    scene->removeItem(text);
}

void TextRect::hide()
{
    text->hide();
    rect->hide();
}

void TextRect::show()
{
    text->show();
    rect->show();
}

void TextRect::setText(QString newText)
{
    text->setPlainText(newText);
    text->setTextWidth(width);
    int h = text->boundingRect().height();
    int w = text->boundingRect().width();
    if (alignment == Center){
        rect->setRect(x - w / 2, y - h / 2, w, h);
        text->setPos(x - w / 2, y - h / 2);
    } else {
        rect->setRect(x, y, w, h);
        text->setPos(x + 2, y);
    }
}

void TextRect::setWidth(int newWidth)
{
    width = newWidth;
    text->setTextWidth(width);
    int h = text->boundingRect().height();
    int w = text->boundingRect().width();
    if (alignment == Center){
        rect->setRect(x - w / 2, y - h / 2, w, h);
        text->setPos(x - w / 2, y - h / 2);
    } else {
        rect->setRect(x, y, w, h);
        text->setPos(x + 2, y);
    }
}

void TextRect::setFont(QFont font)
{
    text->setFont(font);
    setWidth(width);
}
