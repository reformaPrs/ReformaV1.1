#ifndef DECODER_H
#define DECODER_H


#include <QObject>

class QDate;

class Decoder : public QObject
{
    Q_OBJECT
public:
    Decoder();

    QDate* decodeDate(char *code, const int length);
private:
    void deshiftCode(char *code, const int length, int shift, QVector<int> shiftBytes);
    void removeShift(char *code, const int length, int shift, QVector<int> shiftBytes);
    void cyclicShift(char *data, const int length, int shift);
    void reverse(char *p, const int dist);
};

#endif // DECODER_H
