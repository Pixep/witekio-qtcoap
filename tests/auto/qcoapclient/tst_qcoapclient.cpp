#include <QtTest>
#include <QCoreApplication>

// add necessary includes here
#include "qcoapclient.h"
#include "qcoaprequest.h"
#include "qcoapreply.h"

Q_DECLARE_METATYPE(QCoapOperation)
Q_DECLARE_METATYPE(QCoapMessage::QCoapMessageType)

class tst_QCoapClient : public QObject
{
    Q_OBJECT

public:
    tst_QCoapClient();
    ~tst_QCoapClient();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void uniqueTokensAndMessageIds();
    void operations_data();
    void operations();
    void multipleRequests_data();
    void multipleRequests();
    void blockwiseReply_data();
    void blockwiseReply();
    void discover_data();
    void discover();
    void observe_data();
    void observe();
};

tst_QCoapClient::tst_QCoapClient()
{
}

tst_QCoapClient::~tst_QCoapClient()
{
}

void tst_QCoapClient::initTestCase()
{
}

void tst_QCoapClient::cleanupTestCase()
{
}

class QCoapClientForTests : public QCoapClient
{
public:
    QCoapClientForTests() : QCoapClient() {}

    //void addNewRequest(const QCoapRequest& req) { addRequest(req); }
};

void tst_QCoapClient::uniqueTokensAndMessageIds()
{
    /*QCoapClientForTests client;

    QList<QByteArray> tokenList;
    QList<quint16> ids;
    for (int i = 0; i < 100; ++i) {
        QCoapRequest* request = new QCoapRequest();
        client.addNewRequest(request);
        QVERIFY(!tokenList.contains(request->token()));
        QVERIFY(!ids.contains(request->messageId()));
        tokenList.push_back(request->token());
        ids.push_back(request->messageId());
    }*/
}

void tst_QCoapClient::operations_data()
{
    QTest::addColumn<QUrl>("url");

    QTest::newRow("get") << QUrl("coap://172.17.0.3:5683/test");
    QTest::newRow("post") << QUrl("coap://172.17.0.3:5683/test");
    QTest::newRow("put") << QUrl("coap://172.17.0.3:5683/test");
    QTest::newRow("delete") << QUrl("coap://172.17.0.3:5683/test");
}

void tst_QCoapClient::operations()
{
    QFETCH(QUrl, url);

    QFAIL("Uncomment the QFAIL");

    QCoapClientForTests client;
    QCoapRequest request(url);

    QCoapReply* reply = nullptr;
    if (qstrcmp(QTest::currentDataTag(), "get") == 0)
        reply = client.get(request);
    else if (qstrcmp(QTest::currentDataTag(), "post") == 0)
        reply = client.post(request);
    else if (qstrcmp(QTest::currentDataTag(), "put") == 0)
        reply = client.put(request);
    else if (qstrcmp(QTest::currentDataTag(), "delete") == 0)
        reply = client.deleteResource(request);

    QSignalSpy spyReplyFinished(reply, SIGNAL(finished()));
    QTRY_COMPARE_WITH_TIMEOUT(spyReplyFinished.count(), 1, 5000);

    QVERIFY(reply != nullptr);
    QByteArray replyData = reply->readAll();

    if (qstrcmp(QTest::currentDataTag(), "get") == 0) {
        QVERIFY(!replyData.isEmpty());
        QCOMPARE(reply->statusCode(), CONTENT);
    } else if (qstrcmp(QTest::currentDataTag(), "post") == 0) {
        QVERIFY(replyData.isEmpty());
        QCOMPARE(reply->statusCode(), CREATED);
    } else if (qstrcmp(QTest::currentDataTag(), "put") == 0) {
        QVERIFY(replyData.isEmpty());
        QCOMPARE(reply->statusCode(), CHANGED);
    } else if (qstrcmp(QTest::currentDataTag(), "delete") == 0) {
        QVERIFY(replyData.isEmpty());
        QCOMPARE(reply->statusCode(), DELETED);
    }

    delete reply;
}

void tst_QCoapClient::multipleRequests_data()
{
    QTest::addColumn<QUrl>("url");

    QTest::newRow("request") << QUrl("coap://172.17.0.3:5683/test");
}

void tst_QCoapClient::multipleRequests()
{
    QFETCH(QUrl, url);

    QCoapClientForTests client;
    //QCoapRequest request(url);

    QCoapReply* replyGet = client.get(QCoapRequest(url));
    QSignalSpy spyReplyGetFinished(replyGet, SIGNAL(finished()));
    QCoapReply* replyPost = client.post(QCoapRequest(url));
    QSignalSpy spyReplyPostFinished(replyPost, SIGNAL(finished()));
    //QCoapReply* replyPut = client.put(QCoapRequest(url));
    //QSignalSpy spyReplyPutFinished(replyPut, SIGNAL(finished()));
    //QCoapReply* replyDelete = client.deleteResource(QCoapRequest(url));
    //QSignalSpy spyReplyDeleteFinished(replyDelete, SIGNAL(finished()));

    QTRY_COMPARE_WITH_TIMEOUT(spyReplyGetFinished.count(), 1, 5000);
    QTRY_COMPARE_WITH_TIMEOUT(spyReplyPostFinished.count(), 1, 5000);
    //QTRY_COMPARE_WITH_TIMEOUT(spyReplyPutFinished.count(), 1, 5000);
    //QTRY_COMPARE_WITH_TIMEOUT(spyReplyDeleteFinished.count(), 1, 5000);

//    QVERIFY(replyGet != nullptr);
//    QVERIFY(replyPost != nullptr);
//    QVERIFY(replyPut != nullptr);
//    QVERIFY(replyDelete != nullptr);

//    QByteArray replyGetData = replyGet->readAll();
//    QByteArray replyPostData = replyPost->readAll();
//    QByteArray replyPutData = replyPut->readAll();
//    QByteArray replyDeleteData = replyDelete->readAll();

//    qDebug() << "replyGetData : " << replyGetData;
//    qDebug() << "replyPostData : " << replyPostData;
//    qDebug() << "replyPutData : " << replyPutData;
//    qDebug() << "replyDeleteData : " << replyDeleteData;

//    QVERIFY(!replyGetData.isEmpty());
//    QCOMPARE(replyGet->statusCode(), CONTENT);
//    QVERIFY(replyPostData.isEmpty());
//    QCOMPARE(replyPost->statusCode(), CREATED);
//    QVERIFY(replyPutData.isEmpty());
//    QCOMPARE(replyPut->statusCode(), CHANGED);
//    QVERIFY(replyDeleteData.isEmpty());
//    QCOMPARE(replyDelete->statusCode(), DELETED);
}

void tst_QCoapClient::blockwiseReply_data()
{
    QTest::addColumn<QUrl>("url");
    QTest::addColumn<QCoapMessage::QCoapMessageType>("type");
    QTest::addColumn<QByteArray>("replyData");

    QByteArray data;
    data.append("/-------------------------------------------------------------\\\n");
    data.append("|                 RESOURCE BLOCK NO. 1 OF 5                   |\n");
    data.append("|               [each line contains 64 bytes]                 |\n");
    data.append("\\-------------------------------------------------------------/\n");
    data.append("/-------------------------------------------------------------\\\n");
    data.append("|                 RESOURCE BLOCK NO. 2 OF 5                   |\n");
    data.append("|               [each line contains 64 bytes]                 |\n");
    data.append("\\-------------------------------------------------------------/\n");
    data.append("/-------------------------------------------------------------\\\n");
    data.append("|                 RESOURCE BLOCK NO. 3 OF 5                   |\n");
    data.append("|               [each line contains 64 bytes]                 |\n");
    data.append("\\-------------------------------------------------------------/\n");
    data.append("/-------------------------------------------------------------\\\n");
    data.append("|                 RESOURCE BLOCK NO. 4 OF 5                   |\n");
    data.append("|               [each line contains 64 bytes]                 |\n");
    data.append("\\-------------------------------------------------------------/\n");
    data.append("/-------------------------------------------------------------\\\n");
    data.append("|                 RESOURCE BLOCK NO. 5 OF 5                   |\n");
    data.append("|               [each line contains 64 bytes]                 |\n");
    data.append("\\-------------------------------------------------------------/\n");

    QTest::newRow("get_large") << QUrl("coap://172.17.0.3:5683/large")
                               << QCoapMessage::NONCONFIRMABLE
                               << data;
}

void tst_QCoapClient::blockwiseReply()
{
    QFETCH(QUrl, url);
    QFETCH(QCoapMessage::QCoapMessageType, type);
    QFETCH(QByteArray, replyData);

    QFAIL("Put the blockwise test to the client");
    for (int i = 0; i < 10; ++i) {
        QCoapClient client;
        QCoapRequest request(url);

        request.setType(type);
        QCoapReply* reply = client.get(request);
        QSignalSpy spyReplyFinished(reply, SIGNAL(finished()));

        QTRY_COMPARE_WITH_TIMEOUT(spyReplyFinished.count(), 1, 10000);

        QByteArray dataReply = reply->readAll();
        QCOMPARE(dataReply, replyData);
    }
}

void tst_QCoapClient::discover_data()
{
    QTest::addColumn<QUrl>("url");
    QTest::addColumn<int>("resourceNumber");

    QTest::newRow("discover") << QUrl("coap://172.17.0.3:5683/")
                              << 29;
}

void tst_QCoapClient::discover()
{
    QFETCH(QUrl, url);
    QFETCH(int, resourceNumber);

    QFAIL("Broken when protocol added");
    for (int i = 0; i < 20; ++i) {
        qDebug() << i;
        QCoapClientForTests client;

        QList<QCoapResource*> resources = client.discover(url); // /.well-known/core

        QCOMPARE(resources.length(), resourceNumber);
    }
}

void tst_QCoapClient::observe_data()
{
    QTest::addColumn<QUrl>("url");

    QTest::newRow("observe") << QUrl("coap://172.17.0.3:5683/obs");
}

void tst_QCoapClient::observe()
{
    QFETCH(QUrl, url);

    QFAIL("Does not work for instance");
    QCoapClientForTests client;
    QCoapRequest request(url);

    QCoapReply* reply;
    QSignalSpy spyReplyFinished(reply, SIGNAL(notified(const QByteArray&)));

    reply = client.observe(request);

    QTRY_COMPARE_WITH_TIMEOUT(spyReplyFinished.count(), 5, 30000);

    delete reply;
}

QTEST_MAIN(tst_QCoapClient)

#include "tst_qcoapclient.moc"
