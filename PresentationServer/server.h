#pragma once

#include "srvthread.h"

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(const QStringList& fileList, const QString &recordFileName);

private:
    const QStringList& fileList;
    QString recordFileName;
    quint16 slideNumber;
    quint16 slideNumberMinimum;
    quint16 slideNumberMaximum;

    std::list<std::unique_ptr<SrvThread>> threads;

signals:
    void slideNumberChanged(quint16 slideNumber);

public:
    void slideNumberIncrease();
    void slideNumberDecrease();
    void setslideNumberMinimum(qint64 slideNumberMinimum);
    void setslideNumberMaximum(quint16 slideNumberMaximum);
    const quint16 getSlideNumber();

public slots:
    void incomingConnection(qintptr socketDescriptor);

};
