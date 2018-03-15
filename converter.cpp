#include "converter.h"

// Конвертирует double (8 байт) в массив из 8 байт, используя объединение
// (в объединении все поля занимают ОДНО И ТО ЖЕ пространство в памяти,
// поэтому если поместить в 8-байтовое объединение число double, а затем получить из объединения
// массив из 8 байт, то они будут содержать в точности байты этого числа).
QByteArray TypesConverter::toByteArray(double d)
{
    _double *result = (_double *)calloc(1, sizeof(_double));
    result->value = d;
    QByteArray res;
    for (int i = 0; i < 8; i++) {
        res.append(result->bytes[i]);
    }
    free(result);
    return res;
}

// См. выше, только в float 4 байта.
QByteArray TypesConverter::toByteArray(float f)
{
    _single *result = (_single *)calloc(1, sizeof(_single));
    result->value = f;
    QByteArray res;
    for (int i = 0; i < 4; i++) {
        res.append(result->bytes[i]);
    }
    free(result);
    return res;
}

// См. выше, только в quint16 2 байта.
QByteArray TypesConverter::toByteArray(quint16 f)
{
    _uint16 *result = (_uint16 *)calloc(1, sizeof(_uint16));
    result->value = f;
    QByteArray res;
    for (int i = 0; i < 2; i++) {
        res.append(result->bytes[i]);
    }
    free(result);
    return res;
}

// См. выше, только в quint32 4 байта.
QByteArray TypesConverter::toByteArray(quint32 f)
{
    _uint32 *result = (_uint32 *)calloc(1, sizeof(_uint32));
    result->value = f;
    QByteArray res;
    for (int i = 0; i < 4; i++) {
        res.append(result->bytes[i]);
    }
    free(result);
    return res;
}

// Делает с помощью объединения обратную операцию: записывает массив байт (в обратном порядке!)
// в объединение, а затем получает из него число из 4 байтов.
// Методы ниже делают то же самое, только количество байтов варьируется.
quint32 TypesConverter::bytesToUInt32(QByteArray bytes, int start)
{
    _uint32 *result = (_uint32 *)calloc(1, sizeof(_uint32));
    for (int i = 0, j = 3; j >= 0; i++, j--) {
        result->bytes[j] = (quint8)bytes[start + i];
    }
    quint32 res = result->value;
    free(result);
    return res;
}

qint32 TypesConverter::bytesToSInt32(QByteArray bytes, int start)
{
    _sint32 *result = (_sint32 *)calloc(1, sizeof(_sint32));
    for (int i = 0, j = 3; j >= 0; i++, j--) {
        result->bytes[j] = (quint8)bytes[start + i];
    }
    qint32 res = result->value;
    free(result);
    return res;
}

quint16 TypesConverter::bytesToUInt16(QByteArray bytes, int start)
{
    _uint16 *result = (_uint16 *)calloc(1, sizeof(_uint16));
    for (int i = 0, j = 1; j >= 0; i++, j--) {
        result->bytes[j] = (quint8)bytes[start + i];
    }
    quint16 res = result->value;
    free(result);
    return res;
}

qint16 TypesConverter::bytesToSInt16(QByteArray bytes, int start)
{
    _sint16 *result = (_sint16 *)calloc(1, sizeof(_sint16));
    for (int i = 0, j = 1; j >= 0; i++, j--) {
        result->bytes[j] = (quint8)bytes[start + i];
    }
    qint16 res = result->value;
    free(result);
    return res;
}

float TypesConverter::bytesToSingle(QByteArray bytes, int start)
{
    _single *result = (_single *)calloc(1, sizeof(_single));
    for (int i = 0, j = 3; j >= 0; i++, j--) {
        result->bytes[j] = (quint8)bytes[start + i];
    }
    float res = result->value;
    free(result);
    return res;
}

double TypesConverter::bytesToDouble(QByteArray bytes, int start)
{
    _double *result = (_double *)calloc(1, sizeof(_double));
    for (int i = 0, j = 7; j >= 0; i++, j--) {
        result->bytes[j] = (quint8)bytes[start + i];
    }
    double res = result->value;
    free(result);
    return res;
}
