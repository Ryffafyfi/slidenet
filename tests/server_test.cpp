#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <QTcpSocket>
#include <QFile>
#include <QDir>
#include <QDataStream>

void sendTestFile(const QString &serverAddress, quint16 serverPort, const QString &fileName) {
    QTcpSocket socket;
    socket.connectToHost(serverAddress, serverPort);
    if (socket.waitForConnected(3000)) {
        QFile file("test_files/" + fileName); // ���������, ��� ���� ����������
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray fileData = file.readAll();
            QByteArray dataToSend;
            QDataStream out(&dataToSend, QIODevice::WriteOnly);
            out << fileName << fileData;
            socket.write(dataToSend);
            socket.waitForBytesWritten();
            file.close();
        }
        socket.disconnectFromHost();
    }
}

TEST_CASE("Test file reception and saving with correct file name") {
    QString filePath = "received_files/1.jpg";

    // ���������, ��� ���� �� ���������� ����� ������� �����
    if (QFile::exists(filePath)) {
        QFile::remove(filePath);
    }

    // ���������� �������� ���� �� ������
    sendTestFile("127.0.0.1", 12345, "1.jpg");

    // ���������, ��� ���� ��� �������
    CHECK(QFile::exists(filePath));

    // ��������� ���������� �����
    QFile receivedFile(filePath);
    QFile testFile("test_files/1.jpg");
    if (receivedFile.open(QIODevice::ReadOnly) && testFile.open(QIODevice::ReadOnly)) {
        QByteArray receivedData = receivedFile.readAll();
        QByteArray testData = testFile.readAll();
        CHECK(receivedData == testData);
    }
}

TEST_CASE("Test file reception and saving with incorrect file name") {
    QString incorrectFileName = "incorrect_name.jpg";

    // ���������� �������� ���� �� ������
    sendTestFile("127.0.0.1", 12345, incorrectFileName);

    // ���������, ��� ���� �� ��� �������
    CHECK(!QFile::exists("received_files/" + incorrectFileName));
}
