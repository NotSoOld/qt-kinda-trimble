#include "converter.h"

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

QByteArray TypesConverter::toByteArray(unsigned short f)
{
    _int16 *result = (_int16 *)calloc(1, sizeof(_int16));
    result->value = f;
    QByteArray res;
    for (int i = 0; i < 2; i++) {
        res.append(result->bytes[i]);
    }
    free(result);
    return res;
}

QByteArray TypesConverter::toByteArray(unsigned int f)
{
    _int32 *result = (_int32 *)calloc(1, sizeof(_int32));
    result->value = f;
    QByteArray res;
    for (int i = 0; i < 4; i++) {
        res.append(result->bytes[i]);
    }
    free(result);
    return res;
}


unsigned int TypesConverter::bytesToInt32(QByteArray bytes, int start)
{
    _int32 *result = (_int32 *)calloc(1, sizeof(_int32));
    for (int i = start; i < start + 4; i++) {
        result->bytes[i - start] = bytes[i];
    }
    unsigned int res = result->value;
    free(result);
    return res;
}

unsigned short TypesConverter::bytesToInt16(QByteArray bytes, int start)
{
    _int16 *result = (_int16 *)calloc(1, sizeof(_int16));
    for (int i = start; i < start + 2; i++) {
        result->bytes[i - start] = bytes[i];
    }
    unsigned short res = result->value;
    free(result);
    return res;
}

float TypesConverter::bytesToSingle(QByteArray bytes, int start)
{
    _single *result = (_single *)calloc(1, sizeof(_single));
    for (int i = start; i < start + 4; i++) {
        result->bytes[i - start] = bytes[i];
    }
    float res = result->value;
    free(result);
    return res;
}

double TypesConverter::bytesToDouble(QByteArray bytes, int start)
{
    _double *result = (_double *)calloc(1, sizeof(_double));
    for (int i = start; i < start + 8; i++) {
        result->bytes[i - start] = bytes[i];
    }
    double res = result->value;
    free(result);
    return res;
}
