#ifndef QCOAPINTERNALMESSAGE_P_H
#define QCOAPINTERNALMESSAGE_P_H

#include "qcoapinternalmessage.h"
#include "qcoapmessage_p.h"
#include <private/qobject_p.h>

QT_BEGIN_NAMESPACE

class QCoapInternalMessagePrivate : public QObjectPrivate
{
public:
    QCoapInternalMessagePrivate();
    QCoapInternalMessagePrivate(const QCoapInternalMessagePrivate& other);
    ~QCoapInternalMessagePrivate();

    QCoapMessage message;

    uint currentBlockNumber;
    bool hasNextBlock;
    uint blockSize;
};

QT_END_NAMESPACE

#endif // QCOAPINTERNALMESSAGE_P_H
