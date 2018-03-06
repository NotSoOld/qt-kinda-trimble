#include "qbytehelper.h"

void QByteArrayHelper::appendAndStuff(QByteArray *bytes, double d)
{
    QByteArray doubleBytes = TypesConverter::toByteArray(d);
    for (int i = 0; i < 8; i++) {
        appendAndStuff(bytes, (byte)doubleBytes[i]);
    }
}

void QByteArrayHelper::appendAndStuff(QByteArray *bytes, float f)
{
    QByteArray floatBytes = TypesConverter::toByteArray(f);
    for (int i = 0; i < 4; i++) {
        appendAndStuff(bytes, (byte)floatBytes[i]);
    }
}

void QByteArrayHelper::appendAndStuff(QByteArray *bytes, quint16 f)
{
    QByteArray shortBytes = TypesConverter::toByteArray(f);
    for (int i = 0; i < 2; i++) {
        appendAndStuff(bytes, (byte)shortBytes[i]);
    }
}

void QByteArrayHelper::appendAndStuff(QByteArray *bytes, quint32 f)
{
    QByteArray intBytes = TypesConverter::toByteArray(f);
    for (int i = 0; i < 4; i++) {
        appendAndStuff(bytes, (byte)intBytes[i]);
    }
}

void QByteArrayHelper::appendAndStuff(QByteArray *bytes, quint8 b)
{
    //qDebug() << "bytes version called";
    bytes->append(b);
    // DLE в сообщении необходимо "застаффить".
    if (b == DLE) {
        bytes->append(DLE);
    }
}