/****************************************************************************
**
** Copyright (C) 2017 Witekio.
** Contact: https://witekio.com/contact/
**
** This file is part of the QtCoap module.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QCOAPREPLY_H
#define QCOAPREPLY_H

#include <QtCoap/qcoapmessage.h>
#include <QtCoap/qcoaprequest.h>
#include <QtCoap/qcoapglobal.h>
#include <QtCoap/qcoapnamespace.h>
#include <QtCore/qiodevice.h>

QT_BEGIN_NAMESPACE

class QCoapInternalReply;
class QCoapReplyPrivate;
class Q_COAP_EXPORT QCoapReply : public QIODevice
{
    Q_OBJECT
public:

    explicit QCoapReply(const QCoapRequest &request, QObject *parent = nullptr);
    ~QCoapReply();

    QtCoap::StatusCode statusCode() const;
    QCoapMessage message() const;
    QCoapRequest request() const;
    QUrl url() const;
    QtCoap::Method method() const;
    QtCoap::Error errorReceived() const;
    bool isRunning() const;
    bool isFinished() const;
    bool isAborted() const;

public Q_SLOTS:
    void abortRequest();

Q_SIGNALS:
    void finished(QCoapReply *reply);
    void notified(QCoapReply *reply, const QCoapMessage &message);
    void error(QCoapReply *reply, QtCoap::Error error);
    void aborted(const QCoapToken &token);

protected Q_SLOTS:
    void setError(QtCoap::StatusCode statusCode);
    void setError(QtCoap::Error error);

protected:
    friend class QCoapProtocol;
    friend class QCoapProtocolPrivate;

    explicit QCoapReply(QCoapReplyPrivate &dd, QObject *parent = nullptr);

    void setRunning(const QCoapToken &token, QCoapMessageId messageId);
    void setObserveCancelled();
    virtual void setContent(const QCoapInternalReply *internalReply);
    virtual void setNotified(const QCoapInternalReply *internalReply);
    void setFinished(QtCoap::Error error = QtCoap::NoError);
    qint64 readData(char *data, qint64 maxSize) Q_DECL_OVERRIDE;
    qint64 writeData(const char *data, qint64 maxSize) Q_DECL_OVERRIDE;

    Q_DECLARE_PRIVATE(QCoapReply)
};

QT_END_NAMESPACE

#endif // QCOAPREPLY_H
