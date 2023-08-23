#ifndef IMPULSEADDER_H
#define IMPULSEADDER_H

#include <QDialog>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QIcon>

#include <dotsPairs/miosuit.h>

#include <bodyParts/body.h>

/*!
 * \brief Класс диалогового окна для добавления значений импульсов
 */
class ImpulseAdder : public QObject
{
    Q_OBJECT

public:
    explicit ImpulseAdder(QGraphicsScene *scene);
    ~ImpulseAdder();

    /*!
     * \brief setPopBodyPart устанавливает часть тела, которую отрисовывает на сцене
     * \param newPopBodyPart
     */
    void setPopBodyPart(int partID);

    int getPartID() const;
signals:
    /*!
     * \brief done вызывается при закрытии всплывающих мышц
     */
    void done(int);
private:
    Body *body;
    MioSuit *mioSuit;

    QGraphicsScene *scene;
    QGraphicsItemGroup *group;

    int partID; ///< id выбранной части тела

    bool wasBodyPartClicked = false;

    void lastCheck(); ///< проверка перед закрытием
private slots:
    /*!
     * \brief onBodyPartClicked вызывается при получении сигнала от части тела
     * \param isFirstMuscle
     *
     * В результате выделяет мышцы в зависимоти от значений.
     */
    void onBodyPartClicked(bool isFirstMuscle);
public slots:
    /*!
     * \brief onBackgroundClicked вызывается при нажатии на задний фон
     *
     * В результета вызывает закрытие всплывающего окна при дополнительном подтверждении.
     */
    void onBackgroundClicked();
};

#endif // IMPULSEADDER_H
