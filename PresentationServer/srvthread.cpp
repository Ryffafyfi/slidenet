#include "srvthread.h"

SrvThread::SrvThread(qintptr ID, QStringList fileList, const QString& recordFileName, QObject *parent) : QThread(parent),
    socketDescriptor {ID}, fileList {fileList}, recordFileName { recordFileName }, slideNumber { 1 }
{

}

void SrvThread::run()
{
    qDebug() << " Thread started: " << this;

    socket = std::make_unique<QTcpSocket>();

    if(!socket.get()->setSocketDescriptor(this->socketDescriptor))
    {
        emit error(socket->error());
        return;
    }

    connect(socket.get(), &QTcpSocket::disconnected, this, &SrvThread::disconnected);
    connect(socket.get(), &QTcpSocket::readyRead, this, &SrvThread::receivingCommandFromClient, Qt::DirectConnection);

    SendImagesToClient();

    exec();
}

void SrvThread::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";

    socket->deleteLater();
    exit(0);
}

void SrvThread::slideNumberChange(quint16 slideNumber)
{
    this->slideNumber = slideNumber;
}

void SrvThread::receivingCommandFromClient()
{
    Data.clear();
    QDataStream in(socket.get());
    in.setVersion(QDataStream::Qt_6_6);
    if(in.status() == QDataStream::Ok)
    {
        if (socket->bytesAvailable() == (qint64)sizeof(quint16))
        {
            in >> this->commandFromClient;
        }
    }

    executeClientCommand();
}

void SrvThread::executeClientCommand()
{
    qDebug() << "COMMAND: " << this->commandFromClient;

    switch (this->commandFromClient)
    {
        case 5001:
        {
            sendSlideNumberToClient();
            break;
        }
        case 5002:
        {
            sendRecordToClient();
            break;
        }
        default:
            break;
    }
}

void SrvThread::sendSlideNumberToClient()
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_6);
    out << quint16(this->slideNumber);
    socket->write(Data);
}

void SrvThread::sendRecordToClient()
{
    file.setFileName(recordFileName + ".m4a");
    if(!file.open(QIODevice::ReadOnly)) {return; }

    qDebug() << "sendRecordToClient() 2" << file.size();

    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_6);
    out << file.size();
    out << file.readAll();
    socket->write(Data);
    Data.clear();
    file.close();
}

void SrvThread::SendImagesToClient()
{
    while(!fileList.isEmpty())
    {
        file.setFileName(fileList.front());
        fileList.pop_front();
        if(!file.open(QIODevice::ReadOnly)) { continue; }

        Data.clear();
        QDataStream out(&Data, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_6);
        out << file.size();
        out << file.readAll();
        socket->write(Data);
        Data.clear();
        file.close();
    }
}
