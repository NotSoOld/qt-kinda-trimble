#ifndef CONVERTER_H
#define CONVERTER_H

#include "kinda_trimble_shared.h"

class TypesConverter : public QObject
{
    Q_OBJECT
public:
    static QByteArray toByteArray(double);
    static QByteArray toByteArray(float);
    static QByteArray toByteArray(unsigned short);
    static QByteArray toByteArray(unsigned int);

    static unsigned short bytesToUInt16(QByteArray, int);
    static short bytesToSInt16(QByteArray, int);
    static unsigned int bytesToUInt32(QByteArray, int);
    static int bytesToSInt32(QByteArray, int);
    static float bytesToSingle(QByteArray, int);
    static double bytesToDouble(QByteArray, int);

    union _uint16 {
        unsigned short value;
        byte bytes[2];
    };

    union _sint16 {
        short value;
        byte bytes[2];
    };

    union _uint32 {
        unsigned int value;
        byte bytes[4];
    };

    union _sint32 {
        int value;
        byte bytes[4];
    };

    union _double {
        double value;
        byte bytes[8];
    };

    union _single {
        float value;
        byte bytes[4];
    };

};

#endif // CONVERTER_H
