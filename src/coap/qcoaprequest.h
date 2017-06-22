#ifndef QCOAPREQUEST_H
#define QCOAPREQUEST_H

#include <QObject>
#include <QUrl>
#include <QtCore/qglobal.h>
#include <QTime>
#include <QThread>
#include <QSharedDataPointer>

#include "qcoapglobal.h"
#include "qcoapmessage.h"
#include "qcoapconnection.h"
#include "qcoapreply.h"
#include "qcoapprotocol.h"
#include "qcoapinternalrequest_p.h"

QT_BEGIN_NAMESPACE

//class QCoapProtocol;
class QCoapRequestPrivate;
class QCoapRequest : public QCoapMessage
{
public:
    enum QCoapRequestState {
        WAITING,
        SENT,
        REPLIED,
        REPLYCOMPLETE
    };

    QCoapRequest(const QUrl& url = QUrl(),
                 QCoapMessageType type = NONCONFIRMABLE);
    QCoapRequest(const QCoapRequest &other);
    ~QCoapRequest();

    QCoapRequest& operator=(const QCoapRequest& other);

    void sendRequest() const;
    quint16 generateMessageId();
    QByteArray generateToken();
    void addOptionToAskBlock(uint blockNumber);
    void initializeAttributesFrom(QCoapRequest* request);

    QUrl url() const;
    QCoapOperation operation() const;
    QCoapReply* reply() const;
    QCoapConnection* connection() const;
    bool observe() const;
    void setUrl(const QUrl& url);
    void setOperation(QCoapOperation operation);
    void setObserve(bool observe);

// TODO : remove signals and slots
//signals:
    // void notified(const QByteArray& replyData); // NOTE : will certainly be removed
    //void finished(QCoapRequest* request);
    // void replied(); // NOTE : will certainly be removed

protected:
    void parseUri();
    void setReply(QCoapReply* reply);
    void setConnection(QCoapConnection* connection);
    void setProtocol(QCoapProtocol* protocol);
    void setState(QCoapRequestState state);
    void setRequestForAck(quint16 messageId, const QByteArray& payload = QByteArray());
    void setRequestForReset(quint16 messageId);
    void readReply(); // TODO : remove readReply and find another way to test _q_readReply

    /*Q_DECLARE_PRIVATE(QCoapRequest)
    Q_PRIVATE_SLOT(d_func(), void _q_startToSend())
    //Q_PRIVATE_SLOT(d_func(), void _q_readReply())
    //Q_PRIVATE_SLOT(d_func(), void _q_getNextBlock(uint blockAsked))
    //Q_PRIVATE_SLOT(d_func(), void _q_sendAck(quint16 messageId))*/
};

QT_END_NAMESPACE

#endif // QCOAPREQUEST_H
