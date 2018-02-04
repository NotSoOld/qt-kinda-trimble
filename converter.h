#ifndef CONVERTER_H
#define CONVERTER_H

#include <QObject>
#include <QtCore>
#include <QQuickWindow>

class TypesConverter : public QObject
{
    Q_OBJECT
public:
    static QByteArray toByteArray(double);
    static QByteArray toByteArray(float);
    static QByteArray toByteArray(unsigned short);
    static QByteArray toByteArray(unsigned int);

    static unsigned short bytesToInt16(QByteArray, int);
    static unsigned int bytesToInt32(QByteArray, int);
    static float bytesToSingle(QByteArray, int);
    static double bytesToDouble(QByteArray, int);

    union _int16 {
        unsigned short value;
        byte bytes[2];
    };

    union _int32 {
        unsigned int value;
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