#include "server.h"
#include <iostream>
#include <QFile>
#include <QDataStream>
#include <QThread>

Server::Server(QObject *parent) : QTcpServer(parent), currentSlide(0) {
    connect(this, &Server::dataReceived, this, &Server::handleClientData);
    connect(this, &Server::clientDisconnected, this, &Server::removeClient);
}

void Server::startServer() {
    if (!this->listen(QHostAddress::Any, 12345)) {
        std::cerr << "������ �� ����� �����������!" << std::endl;
    } else {
        std::cout << "������ �������!" << std::endl;
    }
}

void Server::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket *clientSocket = new QTcpSocket(this);
    if (clientSocket->setSocketDescriptor(socketDescriptor)) {
        clients.push_back(clientSocket);
        
        connect(clientSocket, &QTcpSocket::readyRead, this, &Server::handleClientData);
        connect(clientSocket, &QTcpSocket::disconnected, this, &Server::clientDisconnected);
        
        std::cout << "����� �����������: " << socketDescriptor << std::endl;
    } else {
        delete clientSocket;
    }
}

void Server::handleClientData() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    QByteArray data = clientSocket->readAll();
    QDataStream in(&data, QIODevice::ReadOnly);
    QString command;
    in >> command;

    if (command == "UPLOAD") {
        QString fileName;
        QByteArray fileContent;
        in >> fileName >> fileContent;
        receiveFile(clientSocket, fileName, fileContent);
    } else if (command == "GET_CURRENT_SLIDE") {
        sendCurrentSlide(clientSocket);
    } else if (command == "CHANGE_SLIDE") {
        int slideNumber;
        in >> slideNumber;
        changeSlide(clientSocket, slideNumber);
    }
}

void Server::receiveFile(QTcpSocket *clientSocket, const QString &fileName, const QByteArray &fileContent) {
    std::regex filePattern(R"((\d+)\.jpg)");
    if (!std::regex_match(fileName.toStdString(), filePattern)) {
        std::cerr << "������: ������������ ��� ����� " << fileName.toStdString() << std::endl;
        clientSocket->write("������������ ��� �����. ���� ������ ���� � ������� '�����.jpg'.\n");
        return;
    }

    QString filePath = "received_files/";
    QDir dir;
    if (!dir.exists(filePath)) {
        dir.mkpath(filePath);
    }
    QString fullPath = filePath + fileName;
    QFile file(fullPath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(fileContent);
        file.close();
        std::cout << "���� ������� �������: " << fullPath.toStdString() << std::endl;
        clientSocket->write("���� ������� �������.\n");
    } else {
        std::cerr << "������ ��� ���������� �����: " << fullPath.toStdString() << std::endl;
        clientSocket->write("������ ��� ���������� �����.\n");
    }
}

void Server::loadPresentationSlides(const QStringList &filePaths) {
    slides.clear(); // ������� ���������� �������
    for (const QString &filePath : filePaths) {
        slides.push_back(QImage(filePath));
    }
    currentSlide = 0; // ������ � ������� ������
}

void Server::broadcastCurrentSlide() {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << qint64(0); // ��������������� ����� ��� ������ ������
    out << slides[currentSlide];
    out.device()->seek(0);
    out << qint64(block.size() - sizeof(qint64));

    for (QTcpSocket* client : clients) {
        client->write(block);
    }
}

void Server::changeSlide(int slideNumber) {
    if (slideNumber < 0 || slideNumber >= slides.size())
        return;
    currentSlide = slideNumber;
    broadcastCurrentSlide();
}


void Server::clientDisconnected(qintptr socketDescriptor) {
    emit clientDisconnected(socketDescriptor);
}


void Server::removeClient(qintptr socketDescriptor) {
    QTcpSocket *clientSocket = findClient(socketDescriptor);
    if (!clientSocket) return;

    clients.removeOne(clientSocket);
    clientSocket->deleteLater();
}

QTcpSocket* Server::findClient(qintptr socketDescriptor) {
    for (QTcpSocket *client : clients) {
        if (client->socketDescriptor() == socketDescriptor) {
            return client;
        }
    }
    return nullptr;
}