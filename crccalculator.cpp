#include "crccalculator.h"

void CRCCalculator::CalculateTable()
{
    unsigned int tempValue = 0;
    unsigned int topBitMask = (1 << 31);

    for (int i = 0; i < 256; i++) {
        tempValue = (i << 24);
        for (int j = 0; j < 8; j++) {
            if (tempValue & topBitMask) {
                tempValue = ((tempValue << 1) ^ polynomial);
            }
            else {
                tempValue = (tempValue << 1);
            }
        }
        table[i] = tempValue;
    }
}

void CRCCalculator::PrintTable()
{
    for (int i = 0; i < 256; i++) {
        QString hexval = QString("%1").arg(table[i], 0, 16);
        qDebug() << hexval;
    }
}

void CRCCalculator::XorMessage()
{
    for (int i = shift; i < arrivedData->length() - 3; i+=4) {
        unsigned char tablePtr = (*arrivedData)[i];
        for (int j = 0; j < 4; j++) {
            (*arrivedData)[i+j] = (*arrivedData)[i+j] ^ ((table[tablePtr] << (24-j*8)) >> 24);
        }
    }
}

unsigned int CRCCalculator::CalculateCRC(QByteArray data)
{
    unsigned int crc = 0xFFFFFFFF;
    for (int i = 4; i > 0; i--) {
        crc ^= data[data.length() - i];
        crc <<= 8;
    }
    return crc;
}

void CRCCalculator::run()
{
    qDebug() << "Thread " << this->shift << " started";
    this->XorMessage();
    qDebug() << "Thread " << this->shift << " finished";
}
