#ifndef INTERVALDOTSPAIR_H
#define INTERVALDOTSPAIR_H

#include "simpledotspair.h"

#include <QString>

/*!
 * \brief Класс пары контактов для интервалов
 *
 * Класс пары контактов с полями для начала и конца интервала в секундах унаследован от SimpleDotsPair
 */

class IntervalDotsPair : public SimpleDotsPair
{
public:
    IntervalDotsPair(int partID, int id, QString name);
    IntervalDotsPair(const SimpleDotsPair *otherDotsPair, QString name);
    IntervalDotsPair(IntervalDotsPair *other);
    ~IntervalDotsPair();

    /*!
     * \brief getStartTime
     * \return startTime - время начала интервала в секундах
     */
    int getStartTime() const;
    /*!
     * \brief setStartTime
     *
     * Меняет время начала интервала в секундах
     */
    void setStartTime(int newStartTime);

    /*!
     * \brief getEndTime
     * \return endTime
     *
     * Fналогично getStartTime()
     */
    int getEndTime() const;
    /*!
     * \brief setEndTime
     * \param newEndTime - время конца интервала в секундах
     *
     * Аналогично setStartTime()
     */
    void setEndTime(int newEndTime);
    /*!
     * \brief getName
     * \return name - название мышцы, на которуюю действуем импульс, для отображения в списке
     */
    const QString &getName() const;

    /*!
     * \brief toString
     * \return Строка с названием мышцы, id, началом и концом интервала
     */
    QString toString();

    /*!
     * \brief equals Сравнивает две пары по id, длинной импульса, началу и концу интервала
     * \param other
     * \return
     */
    bool equals(const IntervalDotsPair &other);

private:
    int startTime; ///< Время начала интервала
    int endTime; ///< Время конца интервала
    QString name; ///< Название отображающееся в IntervalDialog
};

#endif // INTERVALDOTSPAIR_H
