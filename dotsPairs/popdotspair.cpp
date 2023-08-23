#include "popdotspair.h"

PopDotsPair::PopDotsPair(int x1, int y1, int x2, int y2, int radius, int partID, int id, bool firstMuscle,
                   QString hintStr1[2], QString hintStr2[2], int hintX[2], int hintY[2]):
    DotsPair(x1, y1, x2, y2, radius, partID, id, firstMuscle)
{
    const QPoint offset(IMPULSE_ADDER_OFFSET);
    setOffset(offset);
    setAcceptHoverEvents(true);

    connect(getDot1(), SIGNAL(hoverEntered()), this, SLOT(onDotHoverEntered()));
    connect(getDot1(), SIGNAL(hoverLeft()), this, SLOT(onDotHoverLeft()));
    connect(getDot1(), SIGNAL(clicked()), this, SLOT(onDotClicked()));
    connect(getDot2(), SIGNAL(hoverEntered()), this, SLOT(onDotHoverEntered()));
    connect(getDot2(), SIGNAL(hoverLeft()), this, SLOT(onDotHoverLeft()));
    connect(getDot2(), SIGNAL(clicked()), this, SLOT(onDotClicked()));

    slider = new CustomSliderWidget();
    slider->setGeometry((x1 + x2) / 2 + offset.x() - slider->rect().width() / 2,
                        (y1 + y2) / 2 + offset.y() - slider->rect().height() / 2,
                        slider->rect().width(), slider->rect().height());

    connect(slider, SIGNAL(impulseClicked(int)), this, SLOT(onImpulseClicked(int)));

    slider->hide();

    hint1 = new TextRect(hintStr1[0], hintX[0] + offset.x(), hintY[0] + offset.y(), TextRect::Corner);
    hint2 = new TextRect(hintStr2[0], hintX[1] + offset.x(), hintY[1] + offset.y(), TextRect::Corner);
    hint1->setWidth(500);
    hint2->setWidth(500);
    hint1->hide();
    hint2->hide();

    for (int i = 0; i < 2; i++) {
        this->hintStr1[i] = hintStr1[i];
        this->hintStr2[i] = hintStr2[i];
    }
}

PopDotsPair::PopDotsPair(int x1, int y1, int x2, int y2, int radius, int partID, int id, bool firstMuscle, QString hint1[2], QString hint2[2], int hintX[2], int hintY[2], int sliderX, int sliderY):
    PopDotsPair(x1, y1, x2, y2, radius, partID, id, firstMuscle, hint1, hint2, hintX, hintY)
{
    const QPoint offset(IMPULSE_ADDER_OFFSET);
    slider->setGeometry(sliderX + offset.x(), sliderY + offset.y(), slider->rect().width(), slider->rect().height());
}

PopDotsPair::~PopDotsPair()
{
    delete slider;
    delete hint1;
    delete hint2;
}

void PopDotsPair::translate(Language language)
{
    if (language == EN) {
        hint1->setText(hintStr1[EN]);
        hint2->setText(hintStr2[EN]);
    } else {
        hint1->setText(hintStr1[RU]);
        hint2->setText(hintStr2[RU]);
    }
}

void PopDotsPair::onImpulseClicked(int newValue)
{
    if(newValue >= 25 || newValue == 0) {
        DotsPair::setValue(newValue);
        if (newValue != 0) {
            getDot1()->setZValue(500);
            getDot2()->setZValue(500);
        } else {
            getDot1()->setZValue(300);
            getDot2()->setZValue(300);
        }
        DotsPair::show();
    }
}

void PopDotsPair::onDotHoverEntered()
{
    if (sender() == getDot1()){
        if (getValue() == 0)
            getDot1()->setBrush(QColor(HOVERED_DOT_COLOR));
        hint1->show();
    } else {
        if (getValue() == 0)
            getDot2()->setBrush(QColor(HOVERED_DOT_COLOR));
        hint2->show();
    }
    getDot1()->setRadius(getRadius() + 8);
    getDot2()->setRadius(getRadius() + 8);
    getDot1()->setZValue(500);
    getDot2()->setZValue(500);
}

void PopDotsPair::onDotHoverLeft()
{
    if (getValue() == 0)
        getDot1()->setBrush(QColor(DOT_COLOR));
    getDot2()->setBrush(getDot1()->brush());
    getDot1()->setRadius(getRadius() );
    getDot2()->setRadius(getRadius() );
    if (getValue() == 0) {
        getDot1()->setZValue(300);
        getDot2()->setZValue(300);
    } else {
        getDot1()->setZValue(400);
        getDot2()->setZValue(400);
    }
    hint1->hide();
    hint2->hide();

}

void PopDotsPair::onDotClicked()
{
    slider->retranslateUI();
    slider->show();
}

void PopDotsPair::addToScene(QGraphicsScene *scene)
{
    DotsPair::addToScene(scene);
    scene->addWidget(slider);
    slider->graphicsProxyWidget()->setZValue(900);
    slider->setValue(getValue());
    hint1->addToScene(scene);
    hint2->addToScene(scene);
    if (getValue() == 0){
        hide();
    }
}

void PopDotsPair::removeFromScene(QGraphicsScene *scene)
{
    DotsPair::removeFromScene(scene);
    scene->removeItem(slider->graphicsProxyWidget());
    slider->graphicsProxyWidget()->setWidget(NULL);
    hint1->removeFromScene(scene);
    hint2->removeFromScene(scene);
    slider->hide();
}

void PopDotsPair::hide()
{
    DotsPair::hide();
    slider->hide();
}

void PopDotsPair::setValue(int newValue)
{
    DotsPair::setValue(newValue);
    slider->setValue(newValue);
}

