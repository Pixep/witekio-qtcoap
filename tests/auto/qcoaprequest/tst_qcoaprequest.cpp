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

#include <QtTest>
#include <QCoreApplication>

#include <QtCoap/qcoapglobal.h>
#include <QtCoap/qcoapnamespace.h>
#include <QtCoap/qcoaprequest.h>
#include <QtCoap/qcoapconnection.h>

class tst_QCoapRequest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void ctor_data();
    void ctor();
    void setUrl_data();
    void setUrl();
    void setMethod_data();
    void setMethod();
    void enableObserve();
    void copyAndDetach();
};

void tst_QCoapRequest::ctor_data()
{
    QTest::addColumn<QUrl>("url");

    QTest::newRow("empty") << QUrl();
    QTest::newRow("coap") << QUrl("coap://vs0.inf.ethz.ch:5683/test");
}

void tst_QCoapRequest::ctor()
{
    QFETCH(QUrl, url);

    QCoapRequest request(url);
    QCOMPARE(request.url(), url);
}

void tst_QCoapRequest::setUrl_data()
{
    QTest::addColumn<QUrl>("inputUrl");
    QTest::addColumn<QUrl>("expectedUrl");

    QTest::newRow("empty") << QUrl() << QUrl();
    QTest::newRow("coap") << QUrl("coap://10.11.12.13:5683/test") << QUrl("coap://10.11.12.13:5683/test");
    QTest::newRow("other_port") << QUrl("coap://10.11.12.13:8888/test") << QUrl("coap://10.11.12.13:8888/test");
    QTest::newRow("no_port") << QUrl("coap://vs0.inf.ethz.ch/test") << QUrl("coap://vs0.inf.ethz.ch:5683/test");
    QTest::newRow("no_scheme_no_port") << QUrl("vs0.inf.ethz.ch/test") << QUrl("coap://vs0.inf.ethz.ch:5683/test");
    QTest::newRow("incorrect_scheme") << QUrl("http://vs0.inf.ethz.ch:5683/test") << QUrl();
    QTest::newRow("invalid") << QUrl("-coap://vs0.inf.ethz.ch:5683/test") << QUrl();
}

void tst_QCoapRequest::setUrl()
{
    QFETCH(QUrl, inputUrl);
    QFETCH(QUrl, expectedUrl);

    QCoapRequest request;
    request.setUrl(inputUrl);
    QCOMPARE(request.url(), expectedUrl);
}

void tst_QCoapRequest::setMethod_data()
{
    QTest::addColumn<QtCoap::Method>("method");

    QTest::newRow("get") << QtCoap::Get;
    QTest::newRow("put") << QtCoap::Put;
    QTest::newRow("post") << QtCoap::Post;
    QTest::newRow("delete") << QtCoap::Delete;
    QTest::newRow("other") << QtCoap::Other;
}

void tst_QCoapRequest::setMethod()
{
    QFETCH(QtCoap::Method, method);

    QCoapRequest request;
    request.setMethod(method);
    QCOMPARE(request.method(), method);
}

void tst_QCoapRequest::enableObserve()
{
    QCoapRequest request;

    QCOMPARE(request.isObserve(), false);
    request.enableObserve();

    QCOMPARE(request.isObserve(), true);
}

void tst_QCoapRequest::copyAndDetach()
{
    QCoapRequest a;
    a.setMessageId(3);
    a.setPayload("payload");
    a.setToken("token");
    a.setType(QCoapMessage::Acknowledgment);
    a.setVersion(5);
    a.setMethod(QtCoap::Delete);
    QUrl testUrl("coap://url:500/resource");
    a.setUrl(testUrl);
    QUrl testProxyUrl("test://proxyurl");
    a.setProxyUrl(testProxyUrl);

    // Test the QCoapMessage copy
    QCoapMessage b(a);
    QVERIFY2(b.messageId() == 3, "Message not copied correctly");
    QVERIFY2(b.payload() == "payload", "Message not copied correctly");
    QVERIFY2(b.token() == "token", "Message not copied correctly");
    QVERIFY2(b.type() == QCoapMessage::Acknowledgment, "Message not copied correctly");
    QVERIFY2(b.version() == 5, "Message not copied correctly");

    // Test the QCoapRequest copy
    QCoapRequest c(a);
    QVERIFY2(c.method() == QtCoap::Delete, "Request not copied correctly");
    QVERIFY2(c.url() == testUrl, "Request not copied correctly");
    QVERIFY2(c.proxyUrl() == testProxyUrl, "Request not copied correctly");

    // Detach
    c.setMessageId(9);
    QCOMPARE(c.messageId(), 9);
    QCOMPARE(a.messageId(), 3);
}

QTEST_APPLESS_MAIN(tst_QCoapRequest)

#include "tst_qcoaprequest.moc"
