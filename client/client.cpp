#include "client.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

Client::Client(QWidget *parent) : QMainWindow(parent), socket(new QTcpSocket(this)) {
    setWindowTitle("Client");
    resize(300, 200);

    QVBoxLayout *layout = new QVBoxLayout;
    QPushButton *uploadButton = new QPushButton("��������� � ��������� �����", this);
    QLabel *statusLabel = new QLabel("������: ��������", this);

    layout->addWidget(uploadButton);
    layout->addWidget(statusLabel);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    connect(uploadButton, &QPushButton::clicked, this, &Client::selectAndSendFiles);
    connect(socket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &Client::onDataReceived);
}

void Client::connectToServer(const QString &address, quint16 port) {
    socket->connectToHost(address, port);
}

void Client::onConnected() {
    // ��������� ��������� ����������� � �������
}

void Client::onDataReceived() {
    QByteArray data = socket->readAll();
    // ��������� ���������� ������
}

void Client::selectAndSendFiles() {
    QStringList fileNames = QFileDialog::getOpenFileNames(this, "�������� ����� ��� ��������", QString(), "Images (*.jpg)");

    for (const QString &fileName : fileNames) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray fileData = file.readAll();
            QByteArray dataToSend;
            QDataStream out(&dataToSend, QIODevice::WriteOnly);
            out << QFileInfo(fileName).fileName() << fileData;
            socket->write(dataToSend);
            socket->waitForBytesWritten();
            file.close();
        }
    }
}
