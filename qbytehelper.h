#ifndef QBYTEHELPER_H
#define QBYTEHELPER_H

#include "kinda_trimble_shared.h"
#include "converter.h"

// Класс-помощник для превращения многобайтовых величин в массив байт с произведенным экранированием
// байтов DLE 0x10 и присоединением их к пакету (ссылка на него передается первым параметром). Версии методов
// для всех возможных многобайтовых величин, и также для однобайтовой величины.
class QByteArrayHelper : public QObject
{
    Q_OBJECT
public:
    // Метод для однобайтовой величины:
    static void appendAndStuff(QByteArray *, quint8);

    // Методы для многобайтовых величин:
    static void appendAndStuff(QByteArray *, double);
    static void appendAndStuff(QByteArray *, float);
    static void appendAndStuff(QByteArray *, quint16);
    static void appendAndStuff(QByteArray *, quint32);

};

#endif // QBYTEHELPER_H