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

    static quint16 bytesToUInt16(QByteArray, int);
    static qint16 bytesToSInt16(QByteArray, int);
    static quint32 bytesToUInt32(QByteArray, int);
    static qint32 bytesToSInt32(QByteArray, int);
    static float bytesToSingle(QByteArray, int);
    static double bytesToDouble(QByteArray, int);

    union _uint16 {
        quint16 value;
        quint8 bytes[2];
    };

    union _sint16 {
        qint16 value;
        quint8 bytes[2];
    };

    union _uint32 {
        quint32 value;
        quint8 bytes[4];
    };

    union _sint32 {
        qint32 value;
        quint8 bytes[4];
    };

    union _double {
        double value;
        quint8 bytes[8];
    };

    union _single {
        float value;
        quint8 bytes[4];
    };

};

#endif // CONVERTER_H
