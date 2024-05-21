#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDir>

class Server : public QTcpServer {
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    void startServer();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    void handleClient(QTcpSocket *clientSocket);
    void receiveFile(QTcpSocket *clientSocket);

    std::vector<QTcpSocket*> clients;
    std::mutex clientsMutex;
};

#endif // SERVER_H
