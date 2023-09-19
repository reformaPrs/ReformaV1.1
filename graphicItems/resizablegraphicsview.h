#ifndef RESIZABLEGRAPHICSVIEW_H
#define RESIZABLEGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QDebug>
#include <QMouseEvent>

class ResizableGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    ResizableGraphicsView(QWidget *parent = nullptr);
    ResizableGraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr);

signals:
    void clicked();
    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
};

#endif // RESIZABLEGRAPHICSVIEW_H
