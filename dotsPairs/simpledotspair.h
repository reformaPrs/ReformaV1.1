#ifndef SIMPLEDOTSPAIR_H
#define SIMPLEDOTSPAIR_H

#include <QObject>

/*!
 * \brief Базовый класс для пары контактов костюма
 */

class SimpleDotsPair : public QObject
{
public:
    explicit SimpleDotsPair(int partID, int id);
    explicit SimpleDotsPair(SimpleDotsPair *other);
    virtual ~SimpleDotsPair();


    enum PairType{
        SINISTER_DOWN_NORMAL = 0,
        SINISTER_DOWN_ANTAGONIST = 1,
        SINISTER_UP_NORMAL = 10,
        SINISTER_UP_ANTAGONIST = 11,

        DEXTER_DOWN_NORMAL = 100,
        DEXTER_DOWN_ANTAGONIST = 101,
        DEXTER_UP_NORMAL = 110,
        DEXTER_UP_ANTAGONIST = 111,

        NECK = 1000
    };
    Q_ENUM(PairType);

    /*!
     * \brief getValue
     * \return value - значение длинны импульса в мкс
     */
    int getValue() const;

    /*!
     * \brief setValue
     * \param newValue - новое значение длинны импульса в мкс
     */
    virtual void setValue(int newValue);

    /*!
     * \brief getId
     * \return id - номер пары контактов по документации
     */
    int getID() const;

    /*!
     * \brief getPartID
     * \return partID - номер части тела (мышцы), на которой находиться параконтактов
     */
    int getPartID() const;
    bool getIsInDexterGroup() const;

    bool getIsInUpGroup() const;

    bool getIsInAntagonistGroup() const;

    const PairType &getType() const;
    void setType(const PairType &newType);

protected:
    PairType type;

private:

    int value; ///< Значение импульса в мкс
    int partID; ///< Номер части тела соответствующей паре контактов
    int id; ///< Уникальный номер пары контактов
};

#endif // SIMPLEDOTPAIR_H
