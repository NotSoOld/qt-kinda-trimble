#ifndef CRCCALCULATOR_H
#define CRCCALCULATOR_H

#include <QObject>
#include <QtCore>
#include <QThread>
#include <QDebug>

class CRCCalculator : public QThread
{
    Q_OBJECT
public:
    static void CalculateTable();
    static void PrintTable();
    static unsigned int table[256];
    static const unsigned int polynomial = 0x04C11DB7;
    void XorMessage();
    static unsigned int CalculateCRC(QByteArray);
    void run();
    QByteArray *arrivedData;
    int shift;
};

#endif // CRCCALCULATOR_H
