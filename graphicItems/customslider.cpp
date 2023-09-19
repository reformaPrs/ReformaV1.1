#include "customslider.h"
#include "qpainter.h"

#include <QGraphicsBlurEffect>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QResource>
#include <QStyle>

CustomSlider::CustomSlider(QWidget *parent):
    QSlider(parent)
{
    setLayoutDirection(Qt::LeftToRight);
    setSingleStep(0);
    setPageStep(0);
}

void CustomSlider::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    qreal progressRatio = static_cast<qreal>(value() - minimum()) / static_cast<qreal>(maximum() - minimum());
    int progressWidth = static_cast<int>(progressRatio * width());

    QPainterPath backgroundPath;
    backgroundPath.addRoundedRect(0, 0, width(), height(), 10, 10);
    QPixmap backgroundImage(":/backgrounds/backgrounds/customSliderDarkBlueBackground.svg");
    QPixmap scaledBackground = backgroundImage.scaled(width(), height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    painter.setClipPath(backgroundPath);
    painter.drawPixmap(rect(), scaledBackground);

    painter.setClipping(false);

    QPainterPath filledPath;
    filledPath.addRoundedRect(0, 0, progressWidth, height(), 10, 10);
    QPixmap filledImage(":/backgrounds/backgrounds/customSliderBlueBackground.svg");
    QPixmap scaledFilled = filledImage.scaled(progressWidth, height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    painter.setClipPath(filledPath);
    painter.drawPixmap(rect(), scaledFilled);

    painter.setClipping(false);
    painter.end();
}
