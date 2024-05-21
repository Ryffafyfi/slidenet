#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QFileDialog>
#include <QMainWindow>

class Client : public QMainWindow {
    Q_OBJECT
public:
    explicit Client(QWidget *parent = nullptr);
    void connectToServer(const QString &address, quint16 port);

private slots:
    void onConnected();
    void onDataReceived();
    void selectAndSendFiles();

private:
    QTcpSocket *socket;
};

#endif // CLIENT_H
