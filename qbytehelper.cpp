#include "qbytehelper.h"

// При вызове любого из методов для экранирования многобайтовых величин побайтно
// будет сначала вызван метод класса TypesConverter, разбивающий величину на массив байт,
// а затем каждый байт этого массива будет экранирован версией метода appendAndStuff() для однобайтовой величины
// (определен самым последним).

void QByteArrayHelper::appendAndStuff(QByteArray *bytes, double d)
{
    QByteArray doubleBytes = TypesConverter::toByteArray(d);
    for (int i = 0; i < 8; i++) {
        appendAndStuff(bytes, (quint8)doubleBytes[i]);
    }
}

void QByteArrayHelper::appendAndStuff(QByteArray *bytes, float f)
{
    QByteArray floatBytes = TypesConverter::toByteArray(f);
    for (int i = 0; i < 4; i++) {
        appendAndStuff(bytes, (quint8)floatBytes[i]);
    }
}

void QByteArrayHelper::appendAndStuff(QByteArray *bytes, quint16 f)
{
    QByteArray shortBytes = TypesConverter::toByteArray(f);
    for (int i = 0; i < 2; i++) {
        appendAndStuff(bytes, (quint8)shortBytes[i]);
    }
}

void QByteArrayHelper::appendAndStuff(QByteArray *bytes, quint32 f)
{
    QByteArray intBytes = TypesConverter::toByteArray(f);
    for (int i = 0; i < 4; i++) {
        appendAndStuff(bytes, (quint8)intBytes[i]);
    }
}

void QByteArrayHelper::appendAndStuff(QByteArray *bytes, quint8 b)
{
    bytes->append(b);
    // DLE в сообщении необходимо заэкранировать.
    if (b == DLE) {
        bytes->append(DLE);
    }
}