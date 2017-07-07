#ifndef QCOAPINTERNALREQUEST_H
#define QCOAPINTERNALREQUEST_H

#include "qcoapglobal.h"
#include "qcoapinternalmessage.h"
#include "qcoapinternalmessage_p.h"
#include "qcoapconnection.h"

#include <QTimer>

QT_BEGIN_NAMESPACE

class QCoapRequest;
class QCoapInternalRequestPrivate;
class QCoapInternalRequest : public QCoapInternalMessage
{
public:
    QCoapInternalRequest(QObject* parent = Q_NULLPTR);
    QCoapInternalRequest(const QCoapRequest& request, QObject* parent = Q_NULLPTR);

    static QCoapInternalRequest invalidRequest();
    void initForAcknowledgment(quint16 messageId, const QByteArray& token);
    void initForReset(quint16 messageId);

    QByteArray toQByteArray() const;
    quint16 generateMessageId();
    QByteArray generateToken();
    void setRequestToAskBlock(uint blockNumber, uint blockSize);
    void setRequestToSendBlock(uint blockNumber, uint blockSize);

    using QCoapInternalMessage::addOption;
    void addOption(const QCoapOption& option);

    bool isValid() const;
    bool cancelObserve() const;
    QCoapConnection* connection() const;
    QTimer* timer() const;
    uint retransmissionCounter() const;
    void setOperation(QCoapOperation operation);
    void setConnection(QCoapConnection* connection);
    void setCancelObserve(bool cancelObserve);

    void setTimeout(uint timeout);
    void retransmit();

    bool operator<(const QCoapInternalRequest& other) const;

private:
    Q_DECLARE_PRIVATE(QCoapInternalRequest)
    //QCoapInternalRequestPrivate* d_func() const;
};

class QCoapInternalRequestPrivate : public QCoapInternalMessagePrivate
{
public:
    QCoapInternalRequestPrivate();

    QCoapOperation operation;
    QCoapConnection* connection;
    bool isValid;
    bool cancelObserve;
    QByteArray fullPayload;

    uint retransmissionCounter;
    uint timeout;
    QTimer* timer;
};

QT_END_NAMESPACE

#endif // QCOAPINTERNALREQUEST_H
