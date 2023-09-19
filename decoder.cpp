#include "decoder.h"
#include <QDate>

#define SEED "KRNEIFKOARAMEAMM"

Decoder::Decoder()
{

}

void Decoder::reverse(char *p, const int dist)
{
    char *q = p + dist - 1;
    for(;p < q; p++, q--){
        char tmp = *p;
        *p = *q;
        *q = tmp;
    }
}

void Decoder::cyclicShift(char *data, const int length, int shift)
{
    shift = shift % length;
    if (shift < 0) shift += length;
    reverse(data, shift);
    reverse(data + shift, length - shift);
    reverse(data, length);
}

void Decoder::removeShift(char* code, const int length, int shift, QVector<int> shiftBytes)
{
    // Поочерёдно вычитаем/складываем сдвиг из/к массиву кроме shiftByte
    for (int i = 0; i < length; i++) {
        if (shiftBytes.indexOf(i) != -1) continue;
        if (i % 2 == 0) {
            code[i] -= shift;
        } else {
            code[i] += shift;
        }

        if (code[i] > 'Z'){
            code[i] = 'A' + code[i] - 'Z' - 1;
        }
        if (code[i] < 'A'){
            code[i] = 'Z' - 'A' + code[i] + 1;
        }
    }
//    qDebug() << code;
}

void Decoder::deshiftCode(char* code, const int length, int  shift, QVector<int> shiftBytes)
{
    // Добавляем значение сдвига к shiftByte
    code[shiftBytes.last()] -= shift;
    if (code[shiftBytes.at(0)] > 'Z'){
        code[shiftBytes.at(0)] = 'Z' - 'A' + code[shiftBytes.at(0)] - 'Z' + 1;
    }

//    qDebug() << code;

    //Циклически сдвигаем массив (кроме shiftByte)

    char data[length];
    int j = 0;
    for (int i = 0; i < length; i++) {
        if (shiftBytes.indexOf(i) == -1){
            data[j] = code[i];
            j++;
        }
    }
    data[j] = '\0';
//    qDebug() << data;


    cyclicShift(data, j, -shift);

//    qDebug() << data;

    j = 0;
    for (int i = 0; i < length; i++) {
        if (shiftBytes.indexOf(i) == -1) {
            code[i] = data[j];
            j++;
        }
    }
//    qDebug() << code;
}

QDate* Decoder::decodeDate(char *code, const int length)
{
    QVector<int> shiftBytes = {8, 10};
    char seed[] = SEED;
    QVector<int> usedShiftBytes;

    while (shiftBytes.size() > 0){
        usedShiftBytes.append(shiftBytes.last());
        int shift = code[shiftBytes.last()] - seed[shiftBytes.last()];
        if (shift < 0)
            shift += 'Z' - 'A' + 1;
//        qDebug() << shift;

        removeShift(code, length, shift, usedShiftBytes);

        deshiftCode(code, length, shift, usedShiftBytes);
        shiftBytes.removeLast();
    }
    //Циклически сдвигаем массив (кроме shiftByte)


    int numbers[7] = { 0 };
    for (int i = 8; i < length; i++) {
        numbers[i % 8] = code[i] - seed[i];
    }

    for (int i = 0; i < 4; i++){
        numbers[i] = code[2*i] - seed[2*i];
        code[2*i] -= numbers[i];
    }
    numbers[4] = code[12] - seed[12];
    code[12] -= numbers[4];
    numbers[5] = code[14] - seed[14];
    code[14] -= numbers[5];
    numbers[6] = code[15] - seed[15];
    code[15] -= numbers[6];

//    qDebug() << code;
    int year, month, day;
    year = numbers[0] * 1000 + numbers[1] * 100 + numbers[2] * 10 + numbers[3];
    month = numbers[4];
    day = numbers[5] * 10 + numbers[6];

    // Проверяем сходство с сидом
    for (int i = 0; i < 16; i++) {
        if (code[i] != seed[i]){
            return nullptr;
        }
    }
//    qDebug() << code;

    return new QDate(year, month, day);
}
