#pragma once

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QFile>

class SrvThread : public QThread
{
    Q_OBJECT
public:
    explicit SrvThread(qintptr ID, QStringList fileList, const QString& recordFileName, QObject *parent = 0);

    virtual void run() final;

signals:
    void error(QTcpSocket::SocketError socketerror);

public slots:
    void disconnected();
    void slideNumberChange(quint16 slideNumber);
    void receivingCommandFromClient();
    void executeClientCommand();

private:
    void SendImagesToClient();
    void sendSlideNumberToClient();
    void sendRecordToClient();

private:
    qintptr socketDescriptor;    
    QStringList fileList;
    QString recordFileName;
    quint16 slideNumber;
    std::unique_ptr<QTcpSocket> socket;

    QByteArray Data;
    QFile file;

    enum class commandsForServer
    {
        numberOfSlide = 5001
    } cfc;

    quint16 commandFromClient;
};
